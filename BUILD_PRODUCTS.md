# NoeEngine setup installers

The NoeEngine 0.1.0-dev distribution is built from an upstream Godot
4.0.5 RC engine source tree. Original SCons source paths and binary
filenames stay compatible; users receive NoeEngine-branded installers.

## Debian / antiX / Ubuntu

Install the build prerequisites:

```sh
sudo apt update
sudo apt install -y python3 scons build-essential pkg-config dpkg-dev \
  libx11-dev libxcursor-dev libxinerama-dev libxi-dev libxrandr-dev \
  libxrender-dev libgl1-mesa-dev libudev-dev libasound2-dev \
  libpulse-dev libfreetype6-dev libfontconfig1-dev libssl-dev \
  libwayland-dev libxkbcommon-dev libvulkan-dev
```

From the repository root:

```sh
./build.sh linux 2
```

Creates `Products/NoeEngine-setup.deb`. To install:

```sh
sudo apt install ./Products/NoeEngine-setup.deb
```

The installed launch command is `noeengine`.

## Windows setup from Debian

For MinGW-w64 cross-compiling and NSIS installer packaging:

```sh
sudo apt install -y gcc-mingw-w64-x86-64 g++-mingw-w64-x86-64 nsis
./build.sh windows 2
```

Creates `Products/NoeEngine-setup.exe`. The `./build.sh all 2`
command produces both Linux and Windows installers on Linux, assuming
both toolchains are installed. A native Windows build is also available
through `python misc/build_products.py windows --jobs 2` with MSVC,
Python, SCons, and NSIS installed.

## macOS setup

macOS requires a Mac with Xcode command-line tools, Python, SCons,
the appropriate graphics SDK, and `pkgbuild`. Run `./build.sh macos 2`
on macOS to create `Products/NoeEngine-setup.pkg`.

**Debian by itself cannot build a macOS installer** without an Apple SDK
and specially configured cross-compilation toolchain. GitHub Actions has
a dedicated macOS runner for this target.

## Local checks

```sh
bash misc/ci_check.sh
```

This performs shell and Python syntax checks and exercises actual Debian
packaging with a fixture binary. It **does not** prove that the full C++
engine compiles. A successful build requires running `./build.sh` and
smoke-testing the produced editor on its target operating system.

## Distribution notes

Installers are currently **unsigned**. Production releases require
Windows code signing and macOS signing/notarization, as well as tested
runtime library dependencies. Original Godot upstream attribution is
retained in `LICENSE.txt`, `COPYRIGHT.txt`, and packaged notices.

## Native Noqeri game scripting

The Noqeri `.nqr` scripting module is compiled into the engine by SCons
(`module_noqeri_enabled=yes`, enabled automatically alongside the
default GDScript module). No separate code editor, addon, or compiler binary
installation is required for **supported scene-script features**.

Scripts are edited in the existing NoeEngine Script editor and attached
directly to scene nodes. Unsupported Noqeri language constructs currently
produce an error instead of executing. See [native language documentation]
(modules/noqeri/README.md).

After a successful Linux build, test the native scene bridge using:

```sh
bin/godot.linuxbsd.editor.x86_64 --headless --path tests/noqeri_scene --quit
```

The output must include `NOQERI_NATIVE_READY` with a zero exit status.
No installer build or runtime test has been verified successful yet.
