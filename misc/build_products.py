#!/usr/bin/env python3
# Copyright (c) 2026 Noethric.
# SPDX-License-Identifier: LicenseRef-Noethric-Proprietary
"""Build the NoeEngine editor and create OS-native installers in Products/.

Linux host: linux (native), windows (MinGW-w64 cross build).
Windows host: windows (native). macOS host: macos (native).
Use CI native runners to produce all three; a Linux host alone cannot ship a
macOS build without a separately provisioned Apple SDK/osxcross toolchain.
"""
import argparse
import glob
import os
from pathlib import Path
import platform
import shutil
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[1]
PRODUCTS = ROOT / "Products"
VERSION = "0.1.0-dev"
NAME = "NoeEngine"


def command(args, cwd=ROOT):
    print("+", " ".join(str(x) for x in args), flush=True)
    subprocess.run([str(x) for x in args], cwd=str(cwd), check=True)


def host_platform():
    if sys.platform.startswith("linux"):
        return "linux"
    if os.name == "nt":
        return "windows"
    if sys.platform == "darwin":
        return "macos"
    raise RuntimeError("Unsupported host OS")


def check_tool(name):
    if shutil.which(name) is None:
        raise RuntimeError("Required tool not found: " + name)


def find_binary(target):
    patterns = {
        "linux": "godot.linuxbsd.editor.x86_64*",
        "windows": "godot.windows.editor.x86_64.exe",
        "macos": "godot.macos.editor.*",
    }
    candidates = sorted((ROOT / "bin").glob(patterns[target]))
    candidates = [p for p in candidates if p.is_file() and not p.name.endswith(".console.exe")]
    if target == "macos":
        candidates = [p for p in candidates if p.name.endswith(".universal") or p.name.endswith(".x86_64") or p.name.endswith(".arm64")]
        candidates.sort(key=lambda p: (not p.name.endswith(".universal"), p.name))
    if not candidates:
        raise RuntimeError("Editor binary missing for " + target + ". Build first or remove --package-only.")
    return candidates[0]


def compile_editor(target, jobs):
    check_tool("scons")
    host = host_platform()
    if target == "macos" and host != "macos":
        raise RuntimeError("macOS installer requires a macOS build runner and SDK.")
    if target == "linux" and host != "linux":
        raise RuntimeError("Linux builds must run on Linux.")
    if target == "windows" and host not in ("windows", "linux"):
        raise RuntimeError("Windows build requires Windows or Linux with MinGW-w64.")
    args = ["scons", "platform=" + ("linuxbsd" if target == "linux" else target),
            "target=editor", "arch=x86_64", "module_mono_enabled=no",
            "debug_symbols=no", "-j" + str(jobs)]
    if target == "windows" and host == "linux":
        check_tool("x86_64-w64-mingw32-gcc")
        check_tool("x86_64-w64-mingw32-g++")
        args.append("use_mingw=yes")
    command(args)


def write_debian(binary):
    check_tool("dpkg-deb")
    with tempfile.TemporaryDirectory(prefix="noeengine-deb-") as tmp:
        root = Path(tmp) / "package"
        app = root / "usr/lib/noeengine"
        bindir = root / "usr/bin"
        applications = root / "usr/share/applications"
        control = root / "DEBIAN"
        for directory in (app, bindir, applications, control):
            directory.mkdir(parents=True, exist_ok=True)
        shutil.copy2(binary, app / "noeengine")
        (app / "noeengine").chmod(0o755)
        launcher = bindir / "noeengine"
        launcher.write_text('#!/bin/sh\nexec /usr/lib/noeengine/noeengine "$@"\n')
        launcher.chmod(0o755)
        documentation = root / "usr/share/doc/noeengine"
        documentation.mkdir(parents=True, exist_ok=True)
        shutil.copy2(ROOT / "LICENSE.txt", documentation / "copyright")
        for name in ("COPYRIGHT.txt", "NOTICE.md", "NOETHRIC-PROPRIETARY-LICENSE.txt"):
            shutil.copy2(ROOT / name, documentation / name)
        (applications / "noeengine.desktop").write_text(
            "[Desktop Entry]\nType=Application\nName=NoeEngine\n"
            "Exec=noeengine %f\nTerminal=false\nCategories=Development;IDE;\n"
        )
        (control / "control").write_text(
            "Package: noeengine\nVersion: 0.1.0~dev1\n"
            "Section: devel\nPriority: optional\nArchitecture: amd64\n"
            "Maintainer: Local Builder <local@localhost>\n"
            "Description: NoeEngine editor by Noethric (locally built, unsigned)\n"
        )
        destination = PRODUCTS / "NoeEngine-setup.deb"
        command(["dpkg-deb", "--build", "--root-owner-group", root, destination])
    return destination


def write_windows(binary):
    check_tool("makensis")
    with tempfile.TemporaryDirectory(prefix="noeengine-nsis-") as tmp:
        directory = Path(tmp)
        shutil.copy2(binary, directory / "NoeEngine.exe")
        shutil.copy2(ROOT / "LICENSE.txt", directory / "LICENSE.txt")
        shutil.copy2(ROOT / "COPYRIGHT.txt", directory / "COPYRIGHT.txt")
        shutil.copy2(ROOT / "NOTICE.md", directory / "NOTICE.md")
        shutil.copy2(ROOT / "NOETHRIC-PROPRIETARY-LICENSE.txt", directory / "NOETHRIC-PROPRIETARY-LICENSE.txt")
        out = PRODUCTS / "NoeEngine-setup.exe"
        # NSIS strings are double quoted; paths are local trusted build paths.
        nsi = directory / "installer.nsi"
        nsi.write_text(
            'Unicode true\nName "NoeEngine"\nOutFile "' + out.as_posix() + '"\n'
            'InstallDir "$PROGRAMFILES64\\Noethric\\NoeEngine"\nRequestExecutionLevel admin\n'
            'Section "Install"\nSetOutPath "$INSTDIR"\nFile "NoeEngine.exe"\nFile "LICENSE.txt"\nFile "COPYRIGHT.txt"\nFile "NOTICE.md"\nFile "NOETHRIC-PROPRIETARY-LICENSE.txt"\n'
            'WriteUninstaller "$INSTDIR\\Uninstall.exe"\n'
            'CreateShortCut "$DESKTOP\\NoeEngine.lnk" "$INSTDIR\\NoeEngine.exe"\n'
            'SectionEnd\nSection "Uninstall"\n'
            'Delete "$DESKTOP\\NoeEngine.lnk"\n'
            'Delete "$INSTDIR\\NoeEngine.exe"\nDelete "$INSTDIR\\LICENSE.txt"\nDelete "$INSTDIR\\COPYRIGHT.txt"\n'
            'Delete "$INSTDIR\\NOTICE.md"\nDelete "$INSTDIR\\NOETHRIC-PROPRIETARY-LICENSE.txt"\n'
            'Delete "$INSTDIR\\Uninstall.exe"\nRMDir "$INSTDIR"\nSectionEnd\n'
        )
        command(["makensis", "/V2" if os.name == "nt" else "-V2", nsi], directory)
    return out


def write_macos(binary):
    check_tool("pkgbuild")
    with tempfile.TemporaryDirectory(prefix="noeengine-pkg-") as tmp:
        app = Path(tmp) / "NoeEngine.app"
        executable = app / "Contents/MacOS/NoeEngine"
        executable.parent.mkdir(parents=True)
        shutil.copy2(binary, executable)
        executable.chmod(0o755)
        resources = app / "Contents/Resources"
        resources.mkdir(parents=True, exist_ok=True)
        shutil.copy2(ROOT / "LICENSE.txt", resources / "LICENSE.txt")
        shutil.copy2(ROOT / "COPYRIGHT.txt", resources / "COPYRIGHT.txt")
        shutil.copy2(ROOT / "NOTICE.md", resources / "NOTICE.md")
        shutil.copy2(ROOT / "NOETHRIC-PROPRIETARY-LICENSE.txt", resources / "NOETHRIC-PROPRIETARY-LICENSE.txt")
        info = app / "Contents/Info.plist"
        info.write_text(
            '<?xml version="1.0" encoding="UTF-8"?>\n'
            '<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" '
            '"http://www.apple.com/DTDs/PropertyList-1.0.dtd">\n'
            '<plist version="1.0"><dict>'
            '<key>CFBundleExecutable</key><string>NoeEngine</string>'
            '<key>CFBundleIdentifier</key><string>xyz.noethric.noeengine</string>'
            '<key>CFBundleName</key><string>NoeEngine</string>'
            '<key>CFBundlePackageType</key><string>APPL</string>'
            '<key>CFBundleVersion</key><string>0.1.0</string>'
            '</dict></plist>\n'
        )
        destination = PRODUCTS / "NoeEngine-setup.pkg"
        command(["pkgbuild", "--component", app, "--install-location", "/Applications",
                 "--identifier", "xyz.noethric.noeengine", "--version", "0.1.0", destination])
    return destination


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("platform", choices=("linux", "windows", "macos"),
                        help="Target platform for this build")
    parser.add_argument("--package-only", action="store_true",
                        help="Package a previously built editor binary")
    parser.add_argument("-j", "--jobs", type=int, default=max(1, min(4, os.cpu_count() or 1)))
    args = parser.parse_args(argv)
    if args.jobs < 1:
        parser.error("--jobs must be positive")
    PRODUCTS.mkdir(exist_ok=True)
    if not args.package_only:
        compile_editor(args.platform, args.jobs)
    binary = find_binary(args.platform)
    if args.platform == "linux":
        result = write_debian(binary)
    elif args.platform == "windows":
        result = write_windows(binary)
    else:
        result = write_macos(binary)
    print("Installer created:", result)


if __name__ == "__main__":
    try:
        main()
    except (RuntimeError, subprocess.CalledProcessError) as exc:
        print("Build failed:", exc, file=sys.stderr)
        sys.exit(1)
