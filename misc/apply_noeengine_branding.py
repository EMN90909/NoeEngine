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
         '"interface/theme/preset", "NoeEngine Dark",'),
        ('"interface/theme/base_color", Color(0.2, 0.23, 0.31)',
         '"interface/theme/base_color", Color(0.075, 0.08, 0.09)'),
        ('"interface/theme/accent_color", Color(0.41, 0.61, 0.91)',
         '"interface/theme/accent_color", Color(0.72, 0.75, 0.80)'),
    ],
}

SVG_ASSETS = {
    "editor/icons/Godot.svg": (
        '<svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" viewBox="0 0 16 16">'
        '<rect width="16" height="16" rx="4" fill="#292c31"/>'
        '<path d="M4 12V4l8 8V4" fill="none" stroke="#f1f3f5" '
        'stroke-width="2" stroke-linecap="round" stroke-linejoin="round"/></svg>\n'
    ),
    "editor/icons/Logo.svg": (
        '<svg xmlns="http://www.w3.org/2000/svg" width="187" height="69" viewBox="0 0 187 69">'
        '<rect x="2" y="5" width="183" height="59" rx="16" fill="#141519" stroke="#555960" stroke-width="1.5"/>'
        '<rect x="15" y="16" width="39" height="38" rx="10" fill="#292c31"/>'
        '<path d="M23 46V24L46 46V24" fill="none" stroke="#f1f3f5" stroke-width="5" '
        'stroke-linecap="round" stroke-linejoin="round"/>'
        '<path d="M69 27v19m0-19 17 19V27m11 0v19h16m-16-10h13m-13-9h16m12 19V27h16m-16 9h12m17 10V27h17m-17 10h14" '
        'fill="none" stroke="#e6e8eb" stroke-width="3.2" stroke-linecap="round" stroke-linejoin="round"/></svg>\n'
    ),
    "editor/icons/DefaultProjectIcon.svg": (
        '<svg xmlns="http://www.w3.org/2000/svg" width="128" height="128" viewBox="0 0 128 128">'
        '<rect width="128" height="128" rx="26" fill="#16181b"/>'
        '<rect x="12" y="12" width="104" height="104" rx="20" fill="none" stroke="#535861" stroke-width="3"/>'
        '<path d="M35 93V35l58 58V35" fill="none" stroke="#f1f3f5" '
        'stroke-width="12" stroke-linecap="round" stroke-linejoin="round"/></svg>\n'
    ),
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
    dark = (18, 19, 22)
    silver = (234, 237, 241)
    pixels = bytearray(bytes(dark) * width * height)

    def paint(x, y, color):
        if 0 <= x < width and 0 <= y < height:
            start = (y * width + x) * 3
            pixels[start:start + 3] = bytes(color)

    # Contrasting N monogram. The welcome screen has a wider composition.
    left = 85 if splash else 18
    top = 38 if splash else 18
    side = 116 if splash else 92
    stroke = 14 if splash else 11
    for dy in range(side):
        for dx in range(side):
            if dx < stroke or dx >= side - stroke or abs(dx - dy) <= stroke // 2:
                paint(left + dx, top + dy, silver)

    if splash:
        # No system fonts or Pillow required for the offline pre-build.
        glyphs = {
            "N": ("10001","11001","10101","10011","10001","10001","10001"),
            "O": ("01110","10001","10001","10001","10001","10001","01110"),
            "E": ("11111","10000","10000","11110","10000","10000","11111"),
            "G": ("01111","10000","10000","10111","10001","10001","01110"),
            "I": ("11111","00100","00100","00100","00100","00100","11111"),
        }
        for index, letter in enumerate("NOEENGINE"):
            for row, bits in enumerate(glyphs[letter]):
                for col, bit in enumerate(bits):
                    if bit == "1":
                        for oy in range(3):
                            for ox in range(3):
                                paint(227 + index * 20 + col * 3 + ox, 87 + row * 3 + oy, silver)
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
