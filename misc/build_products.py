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
import json
import glob
import os
from pathlib import Path
import platform
import shutil
import subprocess
import tarfile
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[1]
PRODUCTS = ROOT / "Products"
VERSION = "0.1.0-dev"
NAME = "NoeEngine"
NOQERI_COMPAT = ROOT / "modules/noqeri/compatibility.json"


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
        "linux": "noeengine.linuxbsd.editor.x86_64*",
        "windows": "noeengine.windows.editor.x86_64.exe",
        "macos": "noeengine.macos.editor.*",
    }
    candidates = sorted((ROOT / "bin").glob(patterns[target]))
    candidates = [p for p in candidates if p.is_file() and not p.name.endswith(".console.exe")]
    if target == "macos":
        candidates = [p for p in candidates if p.name.endswith(".universal") or p.name.endswith(".x86_64") or p.name.endswith(".arm64")]
        candidates.sort(key=lambda p: (not p.name.endswith(".universal"), p.name))
    if not candidates:
        raise RuntimeError("Editor binary missing for " + target + ". Build first or remove --package-only.")
    return candidates[0]


def verify_toolchain(target, package_only=False):
    """Fail early when the source, SDK or native setup packager is missing."""
    if not (ROOT / "SConstruct").is_file():
        raise RuntimeError("Missing SConstruct: run from a complete source checkout.")
    for source in ("modules/noqeri/noqeri_script.cpp",
                   "modules/noqeri/noqeri_translator.cpp"):
        if not (ROOT / source).is_file():
            raise RuntimeError("Missing native Noqeri module source: " + source)
    try:
        compatibility = json.loads(NOQERI_COMPAT.read_text(encoding="utf-8"))
    except (OSError, ValueError) as exc:
        raise RuntimeError("Cannot read Noqeri compatibility.json: " + str(exc))
    if (compatibility.get("language_version") != "0.7.0" or
            compatibility.get("abi_version") != 2 or
            compatibility.get("release_status") != "development-unreleased"):
        raise RuntimeError("Noqeri compatibility must declare 0.7.0-dev and ABI v2.")
    print("Noqeri 0.7.0-dev (edition 2026, ABI v2): restricted native .nqr bridge.", flush=True)
    host = host_platform()
    if target == "linux" and host != "linux":
        raise RuntimeError("Linux builds require a Linux host.")
    if target == "macos" and host != "macos":
        raise RuntimeError("macOS builds require Xcode and a Mac.")
    if target == "windows" and host not in ("windows", "linux"):
        raise RuntimeError("Windows builds require Windows or Linux with MinGW-w64.")
    if target == "android":
        if host not in ("linux", "macos"):
            raise RuntimeError("Android editor builds require Linux or macOS SDK tooling.")
        sdk = os.environ.get("ANDROID_HOME") or os.environ.get("ANDROID_SDK_ROOT") or str(Path.home() / ".local/share/noeengine/android-sdk")
        root = Path(sdk).expanduser()
        if not (root / "ndk/23.2.8568313").is_dir():
            raise RuntimeError("Android NDK r23c is missing; run ./build.sh android --install-deps or install SDK components.")
        for tool in ("java", "scons"):
            if not package_only:
                check_tool(tool)
        if not (ROOT / "platform/android/java/gradlew").is_file():
            raise RuntimeError("Missing Android Gradle wrapper")
        return
    if target in ("windows", "macos"):
        check_tool({"windows": "makensis", "macos": "pkgbuild"}[target])
    if package_only:
        return
    check_tool("scons")
    if target == "linux":
        check_tool("g++")
        check_tool("pkg-config")
    if target == "windows" and host == "linux":
        for tool in ("x86_64-w64-mingw32-gcc", "x86_64-w64-mingw32-g++",
                     "x86_64-w64-mingw32-windres"):
            check_tool(tool)
    if target == "macos":
        check_tool("xcrun")


def compile_editor(target, jobs, with_tests=False):
    check_tool("scons")
    host = host_platform()
    if target == "android":
        sdk = os.environ.get("ANDROID_HOME") or os.environ.get("ANDROID_SDK_ROOT") or str(Path.home() / ".local/share/noeengine/android-sdk")
        sdk_path = str(Path(sdk).expanduser())
        os.environ["ANDROID_HOME"] = sdk_path
        os.environ["ANDROID_SDK_ROOT"] = sdk_path
        command(["scons", "platform=android", "target=editor", "arch=arm64", "production=yes",
                 "module_mono_enabled=no", "module_noqeri_enabled=yes",
                 "generate_android_binaries=yes", "-j" + str(jobs)])
        # SCons produces the native library; Gradle must package the actual APK.
        # Merely finding an old APK in bin/ is not a substitute for this step.
        command(["./gradlew", "generateGodotEditor", "--no-daemon"],
                cwd=ROOT / "platform/android/java")
        return
    if target == "macos" and host != "macos":
        raise RuntimeError("macOS installer requires a macOS build runner and SDK.")
    if target == "linux" and host != "linux":
        raise RuntimeError("Linux builds must run on Linux.")
    if target == "windows" and host not in ("windows", "linux"):
        raise RuntimeError("Windows build requires Windows or Linux with MinGW-w64.")
    args = ["scons", "platform=" + ("linuxbsd" if target == "linux" else target),
            "target=editor", "arch=x86_64", "module_mono_enabled=no",
            "module_gdscript_enabled=yes", "module_noqeri_enabled=yes",
            "tests=" + ("yes" if with_tests else "no"),
            "debug_symbols=no", "-j" + str(jobs)]
    if target == "windows" and host == "linux":
        check_tool("x86_64-w64-mingw32-gcc")
        check_tool("x86_64-w64-mingw32-g++")
        args.append("use_mingw=yes")
    command(args)


def smoke_test_linux(binary, with_tests=False):
    """Verify the compiled editor and the real native .nqr scene before shipping."""
    command([binary, "--headless", "--version"])
    # The product title must be branded, not only the .deb installer filename.
    help_output = subprocess.run([str(binary), "--headless", "--help"], cwd=ROOT,
                                 capture_output=True, text=True, check=True)
    if "NoeEngine v" not in help_output.stdout:
        raise RuntimeError("Compiled editor still identifies as an upstream engine.")
    if with_tests:
        command([binary, "--headless", "--test", "--test-case=[Noqeri]*"])
    project = ROOT / "tests/noqeri_scene"
    if not (project / "project.godot").is_file():
        raise RuntimeError("Noqeri runtime smoke project is missing.")
    # Check that the editor, not only the bare game runtime, starts correctly.
    command([binary, "--headless", "--editor", "--path", project, "--quit"])
    print("+", binary, "--headless --path", project, flush=True)
    try:
        run = subprocess.run(
            [str(binary), "--headless", "--path", str(project)],
            cwd=str(ROOT), capture_output=True, text=True, timeout=90,
            check=False)
    except subprocess.TimeoutExpired as exc:
        raise RuntimeError("Native Noqeri scene timed out after 90 seconds.") from exc
    output = run.stdout + "\n" + run.stderr
    print(output, flush=True)
    if run.returncode != 0:
        raise RuntimeError("Native Noqeri scene failed, exit code " + str(run.returncode))
    for marker in ("NOQERI_NATIVE_READY", "NOQERI_NATIVE_PROCESS"):
        if marker not in output:
            raise RuntimeError("Noqeri scene failed to emit " + marker)
    if output.count("NOQERI_REPEAT_STEP") != 2:
        raise RuntimeError("Noqeri repeat test failed: expected exactly two iterations.")
    print("Native Noqeri lifecycle and repeat smoke: PASSED", flush=True)


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



def find_android_editor_apk():
    candidates = sorted((ROOT / "bin/android_editor_builds").glob("android_editor*release*.apk"))
    if not candidates:
        raise RuntimeError("Real Android editor APK was not generated; check SCons/Gradle logs.")
    return max(candidates, key=lambda candidate: candidate.stat().st_mtime)


def write_linux_app(binary):
    """A directly runnable AppDir and a portable tarball, not just a Debian installer."""
    import tarfile
    app_dir = PRODUCTS / "NoeEngine.AppDir"
    if app_dir.exists():
        shutil.rmtree(app_dir)
    exe = app_dir / "usr/bin/noeengine"
    exe.parent.mkdir(parents=True)
    shutil.copy2(binary, exe)
    exe.chmod(0o755)
    app_run = app_dir / "AppRun"
    app_run.write_text('#!/bin/sh\nHERE="$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)"\nexec "$HERE/usr/bin/noeengine" "$@"\n')
    app_run.chmod(0o755)
    desktop = app_dir / "NoeEngine.desktop"
    desktop.write_text("[Desktop Entry]\nType=Application\nName=NoeEngine\nExec=AppRun\nIcon=NoeEngine\nTerminal=false\nCategories=Development;IDE;\n")
    icon = ROOT / "icon.png"
    if icon.is_file():
        shutil.copy2(icon, app_dir / "NoeEngine.png")
    for name in ("LICENSE.txt", "NOTICE.md", "NOETHRIC-PROPRIETARY-LICENSE.txt"):
        shutil.copy2(ROOT / name, app_dir / name)
    tar_path = PRODUCTS / "NoeEngine-linux-x86_64.tar.gz"
    with tarfile.open(tar_path, "w:gz") as archive:
        archive.add(app_dir, arcname="NoeEngine.AppDir")
    return tar_path


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("platform", choices=("linux", "windows", "macos", "android"),
                        help="Target platform for this build")
    parser.add_argument("--package-only", action="store_true",
                        help="Package a previously compiled editor (without runtime verification)")
    parser.add_argument("--preflight", action="store_true",
                        help="Check source and target dependencies before compiling")
    parser.add_argument("--tests", action="store_true",
                        help="Build/run native Noqeri unit tests (Linux only)")
    parser.add_argument("-j", "--jobs", type=int, default=2)
    args = parser.parse_args(argv)
    if args.jobs < 1:
        parser.error("--jobs must be positive")
    if args.tests and (args.platform != "linux" or args.package_only):
        parser.error("--tests requires a fresh native Linux editor build")
    verify_toolchain(args.platform, package_only=args.package_only)
    if args.preflight:
        print("Toolchain preflight passed for", args.platform)
        return
    PRODUCTS.mkdir(exist_ok=True)
    if not args.package_only:
        compile_editor(args.platform, args.jobs, with_tests=args.tests)
    if args.platform == "android":
        source = find_android_editor_apk()
        output = PRODUCTS / "NoeEngine-android-arm64.apk"
        shutil.copy2(source, output)
        if output.stat().st_size < 1024 * 1024:
            raise RuntimeError("Android APK is too small to be an editor build.")
        import zipfile
        with zipfile.ZipFile(output) as apk:
            if "AndroidManifest.xml" not in apk.namelist():
                raise RuntimeError("Not a valid Android APK: AndroidManifest.xml missing.")
        print("Android editor APK:", output)
        return
    binary = find_binary(args.platform)
    if not args.package_only and args.platform == "linux":
        smoke_test_linux(binary, with_tests=args.tests)
    elif args.package_only:
        print("Packaging prebuilt binary; runtime verification is not performed.", flush=True)
    if args.platform == "linux":
        result = write_linux_app(binary)
        # Keep legacy .deb setup for supported Debian distributions.
        if shutil.which("dpkg-deb"):
            write_debian(binary)
    elif args.platform == "windows":
        result = write_windows(binary)
    else:
        result = write_macos(binary)
    if not result.is_file() or result.stat().st_size == 0:
        raise RuntimeError("Packaging returned without a nonempty artifact: " + str(result))
    print("Build artifact created:", result, "(", result.stat().st_size, "bytes )")


if __name__ == "__main__":
    try:
        main()
    except (RuntimeError, subprocess.CalledProcessError, OSError) as exc:
        print("Build failed:", exc, file=sys.stderr)
        sys.exit(1)
