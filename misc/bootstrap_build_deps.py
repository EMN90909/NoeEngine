#!/usr/bin/env python3
"""Opt-in, fail-closed toolchain setup for official NoeEngine platforms.

Never runs remote shell scripts. Distro packages come from configured system
repositories; Android command-line ZIPs are SHA-256 pinned to Google's download
page. Android license acceptance remains an interactive user choice.
"""
from __future__ import annotations
import argparse
import hashlib
import os
from pathlib import Path
import platform
import shutil
import subprocess
import sys
import tempfile
import urllib.request
import zipfile

ROOT = Path(__file__).resolve().parents[1]
TOOLS_VERSION = "15859902"
ANDROID_TOOLS = {
    "linux": ("commandlinetools-linux-15859902_latest.zip",
              "4e4c464f145a7512b57d088ac6c278c03c9eea610886b35a5e0804e74eedf583"),
    "darwin-x86_64": ("commandlinetools-mac_x86_64-15859902_latest.zip",
                      "c5a6378ab5cf7e0d5701921405115befff13e9ff7417fb588389338f8bd050f3"),
    "darwin-arm64": ("commandlinetools-mac_arm64-15859902_latest.zip",
                     "835b62a26162b229b441d1f6d4680383815a270809eb33522c0d480fa5002c4e"),
}
ANDROID_PACKAGES = ("platform-tools", "build-tools;35.0.0", "platforms;android-35",
                    "cmake;3.10.2.4988404", "ndk;28.1.13356709")

def checked_zip_extract(source: Path, destination: Path):
    with zipfile.ZipFile(source) as archive:
        for info in archive.infolist():
            name = info.filename
            if name.startswith("/") or "\\" in name or any(
                    part in ("", ".", "..") for part in name.rstrip("/").split("/")):
                raise RuntimeError("Unsafe Android archive entry: " + name)
            if (info.external_attr >> 16) & 0o170000 == 0o120000:
                raise RuntimeError("Android tools ZIP contains a symlink")
        archive.extractall(destination)

def sdk_location():
    return Path(os.environ.get("ANDROID_HOME") or os.environ.get("ANDROID_SDK_ROOT") or
                str(Path.home() / ".local/share/noeengine/android-sdk")).expanduser().resolve()

def install_android_tools(*, download: bool, dry_run: bool):
    sdk = sdk_location()
    manager = sdk / "cmdline-tools/latest/bin/sdkmanager"
    if not manager.exists():
        host = platform.system().lower()
        key = "linux" if host == "linux" else ("darwin-" + platform.machine().lower())
        if key not in ANDROID_TOOLS:
            raise RuntimeError("Android tools auto-bootstrap supports Linux and macOS, "
                               "configure sdkmanager manually on this host.")
        name, sha = ANDROID_TOOLS[key]
        url = "https://dl.google.com/android/repository/" + name
        print("Android command-line tools:", url, "SHA-256:", sha)
        if not download:
            raise RuntimeError("Android SDK not found; rerun with --install-deps.")
        if dry_run:
            print("DRY RUN: download verified Android command-line tools and sdkmanager packages")
            return
        with tempfile.TemporaryDirectory(prefix="noe-android-") as tmp:
            archive = Path(tmp) / name
            with urllib.request.urlopen(url, timeout=60) as stream, archive.open("wb") as out:
                digest = hashlib.sha256()
                while chunk := stream.read(1024 * 1024):
                    digest.update(chunk)
                    out.write(chunk)
            if digest.hexdigest() != sha:
                raise RuntimeError("Android download hash mismatch; archive discarded")
            extracted = Path(tmp) / "extracted"
            extracted.mkdir()
            checked_zip_extract(archive, extracted)
            source = extracted / "cmdline-tools"
            if not (source / "bin/sdkmanager").exists():
                raise RuntimeError("Unexpected Android command-line archive layout")
            manager.parent.parent.mkdir(parents=True, exist_ok=True)
            shutil.move(str(source), str(manager.parent.parent))
    if shutil.which("java") is None:
        raise RuntimeError("JDK 17 is required before running sdkmanager.")
    cmd = [str(manager), "--sdk_root=" + str(sdk), *ANDROID_PACKAGES]
    print("+", " ".join(cmd))
    if not dry_run:
        print("Android SDK licenses must already be accepted or will require your input.")
        subprocess.run(cmd, check=True)
    return sdk

def install_distro_packages(targets: list[str], *, dry_run: bool):
    system = platform.system().lower()
    if system == "darwin":
        missing = [name for name in ("xcodebuild", "xcrun") if not shutil.which(name)]
        if "macos" in targets and missing:
            raise RuntimeError("Install Xcode and accept its license in Apple tools first: " +
                               ", ".join(missing))
        if shutil.which("scons") is None:
            cmd = [sys.executable, "-m", "pip", "install", "--user", "scons>=4,<5"]
            print("+", " ".join(cmd))
            if not dry_run:
                subprocess.run(cmd, check=True)
        return
    if system != "linux":
        raise RuntimeError("Automatic system packages are currently supported on Linux/macOS.")
    if shutil.which("apt-get") is None:
        raise RuntimeError("Automatic packages require an apt-based Linux; install dependencies "
                           "through your distro's package manager.")
    packages = ["python3", "python3-pip", "scons", "build-essential", "pkg-config"]
    if "windows" in targets:
        packages += ["mingw-w64", "nsis"]
    if "android" in targets:
        packages += ["openjdk-17-jdk", "unzip", "curl"]
    if "linux" in targets:
        packages += ["libx11-dev", "libxcursor-dev", "libxinerama-dev",
                     "libxrandr-dev", "libxi-dev", "libgl1-mesa-dev", "libasound2-dev",
                     "libpulse-dev", "libudev-dev"]
    privilege = [] if os.geteuid() == 0 else ["sudo"]
    if privilege and not shutil.which("sudo"):
        raise RuntimeError("System package installation needs sudo; install manually or run as root.")
    for cmd in ([*privilege, "apt-get", "update"],
                [*privilege, "apt-get", "install", "-y", "--no-install-recommends", *packages]):
        print("+", " ".join(cmd))
        if not dry_run:
            subprocess.run(cmd, check=True)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("targets", nargs="+", choices=("linux", "windows", "macos", "android"))
    parser.add_argument("--install-deps", action="store_true")
    parser.add_argument("--dry-run", action="store_true")
    args = parser.parse_args()
    if not args.install_deps:
        print("Dependency installation is opt-in. Add --install-deps to build.sh.")
        return 0
    install_distro_packages(args.targets, dry_run=args.dry_run)
    if "android" in args.targets:
        install_android_tools(download=True, dry_run=args.dry_run)
    return 0

if __name__ == "__main__":
    sys.exit(main())
