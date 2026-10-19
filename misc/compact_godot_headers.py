#!/usr/bin/env python3
# Copyright (c) 2026 Noethric.
# SPDX-License-Identifier: LicenseRef-Noethric-Proprietary
"""Replace only the long standard Godot MIT C-family banner in tracked engine code.

Never strip authorship, MIT copyright lines, or independent third-party headers.
The full upstream license remains in LICENSE.txt and COPYRIGHT.txt.
"""
from __future__ import annotations

import argparse
from pathlib import Path
import re
import subprocess

ROOT = Path(__file__).resolve().parents[1]
BORDER = "/**************************************************************************/"
EXTENSIONS = {".cpp", ".h", ".c", ".cc", ".hpp", ".mm", ".m"}
COPYRIGHT_PATTERN = re.compile(r"^/\*\s*(Copyright \(c\) [^\r\n]+?)\s*\*/\s*$", re.M)

def transform(text: str) -> str:
    if not text.startswith(BORDER):
        return text
    positions = []
    start = 0
    for _ in range(4):
        position = text.find(BORDER, start)
        if position < 0 or position > 3000:
            return text
        positions.append(position)
        start = position + len(BORDER)
    header = text[:start]
    if not all(s in header for s in (
        "GODOT ENGINE", "https://godotengine.org",
        "Godot Engine contributors", "Juan Linietsky, Ariel Manzur",
        "Permission is hereby granted", "The above copyright notice",
        'THE SOFTWARE IS PROVIDED "AS IS"',
    )):
        return text
    holders = [line.rstrip() for line in COPYRIGHT_PATTERN.findall(header)]
    if len(holders) != 2:
        # A custom third-party copyright or modified license is not safe to
        # rewrite with a generic MIT identifier.
        return text
    if "Godot Engine contributors" not in holders[0] or "Juan Linietsky" not in holders[1]:
        return text
    eol = "\r\n" if "\r\n" in header else "\n"
    tail = text[start:].lstrip("\r\n")
    short = (
        "// SPDX-License-Identifier: MIT" + eol
        + "// " + holders[0] + eol
        + "// " + holders[1] + eol
        + "// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md" + eol
        + eol
    )
    return short + tail

def eligible_paths(root: Path):
    raw = subprocess.check_output(["git", "ls-files", "-z"], cwd=root)
    for item in raw.split(b"\0"):
        if not item:
            continue
        relative = Path(item.decode("utf-8", "surrogateescape"))
        if relative.suffix not in EXTENSIONS:
            continue
        if relative.parts[0] in ("thirdparty", ".git"):
            continue
        candidate = root / relative
        if candidate.is_file():
            yield candidate

def run(root: Path, apply: bool) -> list[str]:
    for name in ("LICENSE.txt", "COPYRIGHT.txt", "NOTICE.md"):
        if not (root / name).is_file():
            raise RuntimeError("Refusing to rewrite headers without required attribution file: " + name)
    licensed = (root / "LICENSE.txt").read_text(encoding="utf-8")
    if "Godot Engine contributors" not in licensed or "Permission is hereby granted" not in licensed:
        raise RuntimeError("Upstream MIT license file is missing or altered")
    changed = []
    for path in eligible_paths(root):
        original = path.read_bytes()
        try:
            source = original.decode("utf-8")
        except UnicodeDecodeError:
            continue
        updated = transform(source)
        if updated == source:
            continue
        changed.append(path.relative_to(root).as_posix())
        if apply:
            path.write_bytes(updated.encode("utf-8"))
    return changed

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--apply", action="store_true", help="actually rewrite eligible files")
    options = parser.parse_args()
    changed = run(ROOT, options.apply)
    print("NoeEngine legacy MIT-banner cleanup: " + str(len(changed)) +
          (" rewritten" if options.apply else " candidates (dry run)"))
    for path in changed[:30]:
        print("  " + path)
    if len(changed) > 30:
        print("  ... and " + str(len(changed) - 30) + " additional tracked files")
    if not options.apply:
        print("No files changed; use --apply to compact headers while retaining attribution.")

if __name__ == "__main__":
    main()
