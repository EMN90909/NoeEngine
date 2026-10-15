#!/usr/bin/env python3
"""Deterministic NoeEngine fork rebrand for an authenticated local checkout.

Run: python3 tools/rebrand_to_noeengine.py --dry-run
     python3 tools/rebrand_to_noeengine.py --apply

This changes the public project format and binding names; run full platform
builds and compatibility tests before merging an applied migration.
"""
import argparse
from pathlib import Path
import re
import subprocess

REPLACEMENTS = re.compile(r"godot[ ]+engine|godotengine|godot", flags=re.IGNORECASE)
UPSTREAM_FILES = {
    "COPYRIGHT.txt", "AUTHORS.md", "DONORS.md", "LOGO_LICENSE.md",
    "UPSTREAM-MIT-LICENSE.txt", "LICENSE.txt", ".mailmap",
    "README.md", "CHANGELOG.md", "CONTRIBUTING.md",
    "NOEENGINE-LICENSING.md", "NOEENGINE-MIGRATION-AUDIT.md",
    "NOEENGINE-REBRANDING.md",
}
BINARY_SUFFIXES = {
    ".png", ".jpg", ".jpeg", ".gif", ".webp", ".bmp", ".ico",
    ".icns", ".ttf", ".otf", ".woff", ".woff2", ".pdf", ".jar",
    ".a", ".so", ".dll", ".exe", ".zip", ".gz", ".pck",
}
LEGAL_LINE = re.compile(
    r"copyright\b|spdx-license|permission is hereby granted|"
    r"all rights reserved|https?://(?:[^/ ]*\.)?godotengine\.org|"
    r"https?://github\.com/godotengine/|"
    r"https?://docs\.godotengine\.org", re.IGNORECASE
)


def name_replace(text):
    def replacement(match):
        old = match.group(0)
        if old.isupper():
            return "NOEENGINE"
        if old.islower():
            return "noeengine"
        return "NoeEngine"
    return REPLACEMENTS.sub(replacement, text)


def tracked_paths():
    raw = subprocess.check_output(["git", "ls-files", "-z"])
    return [Path(x.decode()) for x in raw.split(b"\0") if x]


def rewrite_text(path):
    if path == Path("tools/rebrand_to_noeengine.py"):
        return None
    if ((path.parent == Path(".") and path.name in UPSTREAM_FILES)
            or path.name.lower().startswith(("license", "copying"))
            or path.suffix.lower() in BINARY_SUFFIXES):
        return None
    raw = path.read_bytes()
    if b"\0" in raw:
        return None
    try:
        old = raw.decode("utf-8")
    except UnicodeError:
        return None
    if not REPLACEMENTS.search(old):
        return None
    updated = "".join(
        line if LEGAL_LINE.search(line) else name_replace(line)
        for line in old.splitlines(keepends=True)
    )
    return updated if updated != old else None


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--dry-run", action="store_true")
    parser.add_argument("--apply", action="store_true")
    args = parser.parse_args()
    if args.dry_run == args.apply:
        parser.error("Specify exactly one of --dry-run or --apply")

    paths = tracked_paths()
    old_names = set(str(x) for x in paths)
    moves = [(p, Path(name_replace(str(p)))) for p in paths
             if name_replace(str(p)) != str(p)]
    target_names = [str(dst) for _, dst in moves]
    if len(target_names) != len(set(target_names)):
        raise RuntimeError("Destination path collision")
    for old, new in moves:
        if str(new) in old_names and str(new) != str(old):
            raise RuntimeError(f"Existing target collision: {new}")

    rewrites = [(p, new) for p in paths
                if p.is_file() and (new := rewrite_text(p)) is not None]
    print(f"Path moves: {len(moves)}; files with text rewrites: {len(rewrites)}")
    if args.dry_run:
        for old, new in moves:
            print(f"{old} -> {new}")
        return

    for p, contents in rewrites:
        p.write_text(contents, encoding="utf-8", newline="")
    for old, new in sorted(moves, key=lambda item: len(item[0].parts), reverse=True):
        new.parent.mkdir(parents=True, exist_ok=True)
        subprocess.run(["git", "mv", "--", str(old), str(new)], check=True)
    remaining = [p for p in tracked_paths() if REPLACEMENTS.search(str(p))]
    if remaining:
        raise RuntimeError(f"Renamed path check failed: {len(remaining)} remaining")
    print("Rebrand applied. Review 'git diff', license notices, platform builds,")
    print("assembly names, package identities, icon artwork, and project migrations.")


if __name__ == "__main__":
    main()
