#!/usr/bin/env python3
"""Static path/license smoke checks for a local NoeEngine git checkout.

Usage: python3 tools/validate_noeengine_paths.py

This does not compile the engine or prove binary, ABI, or serialization
compatibility. Platform build and runtime tests are still required.
"""
from __future__ import annotations

from pathlib import Path
import posixpath
import re
import subprocess
import sys


def main() -> int:
    try:
        raw = subprocess.check_output(["git", "ls-files", "-z"])
    except (FileNotFoundError, subprocess.CalledProcessError):
        print("ERROR: run in a Git checkout with git installed", file=sys.stderr)
        return 2
    files = [p.decode("utf-8") for p in raw.split(b"\0") if p]
    names = set(files)
    errors = []
    for path in files:
        if re.search(r"godot|noeengineengine", path, re.IGNORECASE):
            errors.append(f"Unmigrated or doubled engine name in tracked path: {path}")
        if not Path(path).is_file():
            errors.append(f"Tracked file missing on disk: {path}")
    sln_count = ref_count = android_count = 0
    for path in files:
        if path.endswith(".sln"):
            sln_count += 1
            contents = Path(path).read_text(encoding="utf-8-sig")
            for entry in re.findall(
                r'Project\("\{[^}]+\}"\)\s*=\s*"[^"]+",\s*"([^"]+\.csproj)"',
                contents, re.IGNORECASE,
            ):
                ref_count += 1
                full = posixpath.normpath(posixpath.join(
                    posixpath.dirname(path), entry.replace("\\", "/")
                ))
                if full not in names:
                    errors.append(f"Missing .NET project for {path}: {full}")
        if not path.endswith((".java", ".kt")):
            continue
        roots = (
            "platform/android/java/lib/src/",
            "platform/android/java/app/src/",
            "platform/android/java/editor/src/main/java/",
        )
        root = next((r for r in roots if path.startswith(r)), None)
        if root is None:
            continue
        android_count += 1
        contents = Path(path).read_text(encoding="utf-8")
        match = re.search(r"(?m)^\s*package\s+([\w.]+)", contents)
        expected = path[len(root):].rsplit("/", 1)[0].replace("/", ".")
        if not match or match.group(1) != expected:
            errors.append(
                f"Android package mismatch: {path} "
                f"(declared {match.group(1) if match else 'NONE'}, expected {expected})"
            )
    mit_path = Path("UPSTREAM-MIT-LICENSE.txt")
    mit = mit_path.read_text(encoding="utf-8") if mit_path.exists() else ""
    if ("Permission is hereby granted" not in mit or
            "Godot Engine contributors" not in mit):
        errors.append("Required upstream Godot MIT license/copyright notice missing")
    own_path = Path("LICENSE.txt")
    own = own_path.read_text(encoding="utf-8") if own_path.exists() else ""
    if "PROPRIETARY ADDITIONS" not in own:
        errors.append("NoeEngine original-additions licensing notice missing")
    for issue in errors:
        print("FAIL:", issue)
    print(
        f"Files: {len(files)}; .NET solutions: {sln_count}; "
        f".NET project references: {ref_count}; "
        f"Android Java/Kotlin packages: {android_count}; errors: {len(errors)}"
    )
    if errors:
        return 1
    print("PASS: static tracked-path, .NET reference, Android package and legal checks")
    print("NOTE: compile, boot, editor, export and API compatibility tests not performed")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
