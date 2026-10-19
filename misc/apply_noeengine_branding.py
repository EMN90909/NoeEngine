#!/usr/bin/env python3
# Copyright (c) 2026 Noethric.
# SPDX-License-Identifier: LicenseRef-Noethric-Proprietary
"""Apply safe product branding before building NoeEngine.

Never globally rewrite 'Godot': project.godot, GDScript, native identifiers,
upstream MIT copyright notices and attribution must remain unchanged.
"""
import argparse
from pathlib import Path
import struct
import zlib

ROOT = Path(__file__).resolve().parents[1]

# Keep the build-time task idempotent, including for older source checkouts.
REPLACEMENTS = {
    "version.py": [('short_name = "godot"', 'short_name = "noeengine"')],
    "platform/linuxbsd/SCsub": [('"#bin/godot"', '"#bin/noeengine"')],
    "platform/windows/SCsub": [('"#bin/godot"', '"#bin/noeengine"')],
    "platform/macos/SCsub": [('"#bin/godot"', '"#bin/noeengine"')],
    "misc/build_products.py": [
        ('"godot.linuxbsd.editor.x86_64*"', '"noeengine.linuxbsd.editor.x86_64*"'),
        ('"godot.windows.editor.x86_64.exe"', '"noeengine.windows.editor.x86_64.exe"'),
        ('"godot.macos.editor.*"', '"noeengine.macos.editor.*"'),
    ],
    "editor/editor_node.cpp": [
        ('TTR("About Godot")', 'TTR("About NoeEngine")'),
        ('"Godot v" + String(VERSION_FULL_CONFIG)', '"NoeEngine v" + String(VERSION_FULL_CONFIG)'),
    ],
    "editor/editor_settings.cpp": [
        ('"interface/theme/preset", "Default",',
         '"interface/theme/preset", "NoeEngine Soft",'),
        ('"interface/theme/base_color", Color(0.2, 0.23, 0.31)',
         '"interface/theme/base_color", Color(0.075, 0.08, 0.09)'),
        ('"interface/theme/accent_color", Color(0.41, 0.61, 0.91)',
         '"interface/theme/accent_color", Color(0.72, 0.75, 0.80)'),
    ],
}

# Keep generated editor icons in sync with the high-resolution source emblem.
# This intentionally retains upstream Godot identifiers required by the engine.
VIPER_ICON = (ROOT / "editor/icons/NoeViper.svg").read_text(encoding="utf-8")
SVG_ASSETS = {
    "editor/icons/Godot.svg": VIPER_ICON,
    "editor/icons/Logo.svg": VIPER_ICON,
    "editor/icons/DefaultProjectIcon.svg": VIPER_ICON,
    "editor/icons/GodotMonochrome.svg": VIPER_ICON,
    "editor/icons/GodotFile.svg": VIPER_ICON,
}



def apply_sources(check):
    changed = []
    for relative, edits in REPLACEMENTS.items():
        path = ROOT / relative
        original = path.read_text(encoding="utf-8")
        result = original
        for before, after in edits:
            if before in result:
                result = result.replace(before, after)
            elif after not in result:
                raise RuntimeError(f"Missing branding anchor in {relative}: {before!r}")
        if result != original:
            if check:
                raise RuntimeError(f"Unapplied branding in {relative}")
            path.write_text(result, encoding="utf-8")
            changed.append(relative)
    for relative, svg in SVG_ASSETS.items():
        path = ROOT / relative
        if path.read_text(encoding="utf-8") != svg:
            if check:
                raise RuntimeError(f"Unbranded SVG icon: {relative}")
            path.write_text(svg, encoding="utf-8")
            changed.append(relative)
    return changed


def png_image(width, height, splash=False):
    """Render the supplied vector viper without Pillow or OS font dependencies.

    Scanline filling the simple M/L/Z SVG polygons keeps offline builds
    reproducible on the user's Debian machine and on GitHub Actions.
    """
    import re
    dark = (18, 19, 22)
    pixels = bytearray(bytes(dark) * width * height)
    icon_size = min(height - (6 if splash else 10), width - 10)
    left = (width - icon_size) // 2
    top = (height - icon_size) // 2
    paths = re.findall(r'<path\b[^>]*\bd="([^"]+)"', VIPER_ICON)
    colors = ((249, 250, 255), (255, 124, 5))

    for path, rgb in zip(paths, colors):
        polygons = []
        for fragment in re.findall(r'M([^Z]+)Z', path):
            points = [tuple(map(int, pair.split(","))) for pair in fragment.strip().split()]
            if len(points) >= 3:
                polygons.append(points)
        for row in range(icon_size):
            y = (row + .5) * 400 / icon_size
            crosses = []
            for polygon in polygons:
                for (ax, ay), (bx, by) in zip(polygon, polygon[1:] + polygon[:1]):
                    if (ay <= y < by) or (by <= y < ay):
                        crosses.append(ax + (y - ay) * (bx - ax) / (by - ay))
            crosses.sort()
            for i in range(0, len(crosses) - 1, 2):
                start = max(0, int(crosses[i] * icon_size / 400))
                end = min(icon_size, int(crosses[i + 1] * icon_size / 400))
                for x in range(start, end):
                    at = ((top + row) * width + (left + x)) * 3
                    if rgb == colors[1]:
                        tint = row / max(1, icon_size)
                        pixels[at:at + 3] = bytes((255, int(161 - 68 * tint), int(10 - 5 * tint)))
                    else:
                        pixels[at:at + 3] = bytes(rgb)

    raw = b"".join(b"\x00" + pixels[y * width * 3:(y + 1) * width * 3] for y in range(height))

    def chunk(kind, data):
        return (struct.pack(">I", len(data)) + kind + data
                + struct.pack(">I", zlib.crc32(kind + data) & 0xFFFFFFFF))

    return (b"\x89PNG\r\n\x1a\n"
            + chunk(b"IHDR", struct.pack(">2I5B", width, height, 8, 2, 0, 0, 0))
            + chunk(b"IDAT", zlib.compress(raw, 9))
            + chunk(b"IEND", b""))


def write_splash_assets():
    for relative, width, height, splash in (
        ("main/splash.png", 500, 192, True),
        ("main/splash_editor.png", 500, 192, True),
        ("main/app_icon.png", 128, 128, False),
    ):
        path = ROOT / relative
        data = png_image(width, height, splash)
        if not path.exists() or path.read_bytes() != data:
            path.write_bytes(data)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--check", action="store_true", help="Only check committed text/source branding")
    opts = parser.parse_args()
    changed = apply_sources(opts.check)
    if not opts.check:
        write_splash_assets()
    print("[Branding] NoeEngine source branding " + ("verified" if opts.check else "applied")
          + (": " + ", ".join(changed) if changed else ""))


if __name__ == "__main__":
    main()
