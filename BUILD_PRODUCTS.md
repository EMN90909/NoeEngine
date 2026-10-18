# NoeEngine multi-platform builds

## Build targets

| Command | Host | Real output in `Products/` |
| --- | --- | --- |
| `./build.sh linux 2` | Linux | `NoeEngine.AppDir/AppRun`, `NoeEngine-linux-x86_64.tar.gz`, and optional legacy `.deb` |
| `./build.sh windows 2` | Windows, or Linux with MinGW-w64/NSIS | `NoeEngine-setup.exe` |
| `./build.sh macos 2` | **macOS with Xcode only** | `NoeEngine-setup.pkg` |
| `./build.sh android 2` | Linux/macOS with JDK and Android SDK/NDK | `NoeEngine-android-arm64.apk` (actual Android editor build) |
| `./build.sh all 2` | Linux or macOS | Linux: Linux+Windows+Android. macOS: macOS+Android. Missing SDKs fail explicitly |
| `./build.sh check` | Supported host | Offline lint/tests, **no** C++ compilation |
| `./build.sh patch-tools` | Host with Go 1.23+ | Locally built and tested signed updater/signer executables |

Add `--install-deps` to explicitly download and install toolchains from trusted package repositories and, for Android, Google's official SDK ZIP with a **pinned SHA-256 checksum**. The script does not run downloaded shell scripts. Android `sdkmanager` may request license agreement acceptance, which is the user's decision. The optional installer does not accept those licenses automatically. It may require `sudo` and several gigabytes of bandwidth/storage.

```sh
./build.sh linux 2 --install-deps
./build.sh windows 2 --install-deps
./build.sh android 2 --install-deps
./build.sh macos 2 --install-deps  # Only on a Mac with Xcode installed
```

For offline builds without `--install-deps`, the script checks what is installed and stops with a missing-prerequisite error. Do **not** mistake preflight, a packaging fixture, or a template APK for a compiled, tested application.

**Linux:** The AppDir contains the real compiled editor, an executable launcher and the icon. Unpack the archive and run `NoeEngine.AppDir/AppRun`. It is a directly runnable application, but not guaranteed to be self-contained on every Linux distribution; system runtime libraries may still be required.

**Windows:** Generates an NSIS `.exe` installer with an uninstaller. Cross-compile on Linux via MinGW-w64. Test the executable on Windows before release.

**macOS:** Generates an unsigned `.pkg` with a real `NoeEngine.app` only on a Mac with Xcode/Apple SDK. Linux cannot legitimately download an Apple SDK via this script; macOS signing/notarization must be handled separately.

**Android:** Builds the **editor**, not a desktop binary renamed APK. Uses `scons platform=android target=editor arch=arm64 production=yes generate_android_binaries=yes` and the bundled Gradle wrapper, copies the generated release `.apk` and checks it is a real APK with `AndroidManifest.xml`. Requires Google's Android SDK tools, NDK r28b and JDK 17. A release signing configuration and installation/on-device UX tests are needed before publishing through an app store.

**Branding:** The four root `icon*.png` and `icon*.svg` files use the supplied orange cobra. SVGs are self-contained SVG containers with an embedded optimized raster source, not hand-traced vector paths. Their legibility at all densities needs GUI review, especially macOS and Android launch icons.

## Release gate

Run `./build.sh check` first, then a complete native build for each OS/architecture, execute the real binary, and inspect installation and recovery paths. The repository does not currently contain four verified cross-platform outputs; `build.sh` creates them only on hosts with the required SDKs.

## Original build/package notes

# NoeEngine setup installers

## One-command source build and installer

From the repository root, after installing the platform dependencies below:

```bash
./build.sh                 # build/smoke Linux editor, create Products/NoeEngine-setup.deb
./build.sh check           # run shell/Python tests only, no C++ compilation
./build.sh linux 2 --tests # build Noqeri C++ unit tests, run .nqr scene, then package
./build.sh all 2           # on Linux, requires MinGW-w64 + NSIS for Windows setup too
```

The script stops on failed local checks, missing toolchains, SCons errors,
missing native Noqeri symbols, executable startup failures, and failing
Noqeri scene callbacks. It **does not** claim the build succeeded without
actually compiling. Results and logs go into `Products/`:

- `NoeEngine-setup.deb` (Linux)
- `NoeEngine-setup.exe` (Windows, Linux cross-compile requires MinGW-w64)
- `NoeEngine-setup.pkg` (native macOS only)
- `preflight.log`, `build-linux.log`, etc.

Do not treat a `--package-only` build as a runtime-tested release.

**Noqeri compatibility reference:** upstream source version **0.7.0
development/unreleased**, edition **2026**, ABI **v2**, at commit
`a577578ec48bff94bd66f133ad39349b926fedd9`. The compiler and
game SDK are not certified for complete native execution. NoeEngine's
`.nqr` scene bridge is deliberately a smaller, independently written
feature subset and does not bundle the GPL compiler.


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
sudo apt install -y gcc-mingw-w64-x86-64 g++-mingw-w64-x86-64 binutils-mingw-w64-x86-64 nsis
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
bin/godot.linuxbsd.editor.x86_64 --headless --path tests/noqeri_scene
```

The output must include `NOQERI_NATIVE_READY` with a zero exit status.
No installer build or runtime test has been verified successful yet.

## Capture the actual running editor on Debian/antiX

Once you have successfully compiled and launched **NoeEngine** in a graphical X11 session, run:

```sh
bash misc/capture_editor_snapshot.sh
```

When prompted, focus the NoeEngine Script editor during the five-second countdown. This saves `Products/NoeEngine-editor-snapshot.png`. The script refuses to overwrite an existing screenshot and requires `xfce4-screenshooter` (or ImageMagick + xdotool).

Reference images from upstream Godot documentation are **not** evidence that the current NoeEngine build has rendered successfully.
