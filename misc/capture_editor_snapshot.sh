#!/usr/bin/env bash
# Copyright (c) 2026 Noethric.
# SPDX-License-Identifier: LicenseRef-Noethric-Proprietary
# Capture an actual visible NoeEngine editor window on Linux/X11.
set -Eeuo pipefail

cd "$(dirname "$0")/.."
mkdir -p Products
outfile="Products/NoeEngine-editor-snapshot.png"
if [[ "$#" -gt 0 ]]; then
    outfile="$1"
fi
if [[ "$outfile" != /* ]]; then
    outfile="$PWD/$outfile"
fi
if [[ -e "$outfile" ]]; then
    echo "Refusing to overwrite existing screenshot: $outfile" >&2
    exit 1
fi
if [[ -z "$(printenv DISPLAY 2>/dev/null || :)" ]]; then
    echo "DISPLAY is unset. Open a graphical X11 session and the editor first." >&2
    exit 1
fi
if command -v xfce4-screenshooter >/dev/null 2>&1; then
    mode=xfce
elif command -v xdotool >/dev/null 2>&1 && command -v import >/dev/null 2>&1; then
    mode=imagemagick
else
    echo "Install xfce4-screenshooter or (xdotool + imagemagick)." >&2
    exit 1
fi

echo "Open NoeEngine, switch to the Script editor, then return here and press Enter."
read -r _
echo "Focus the NoeEngine editor window within five seconds..."
sleep 5
if [[ "$mode" == xfce ]]; then
    xfce4-screenshooter --window --save "$outfile"
else
    active_window="$(xdotool getactivewindow)"
    import -window "$active_window" "$outfile"
fi
if [[ ! -s "$outfile" ]]; then
    echo "Screenshot capture did not create a non-empty image." >&2
    exit 1
fi
echo "Actual editor screenshot saved: $outfile"
