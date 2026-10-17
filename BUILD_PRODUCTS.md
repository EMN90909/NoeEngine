# Editor setup installers

Run from the repository root. Builds go through **SCons**, not CMake.

On Debian/Ubuntu, install build dependencies first:

```sh
sudo apt update
sudo apt install -y python3 scons build-essential pkg-config dpkg-dev \
  libx11-dev libxcursor-dev libxinerama-dev libxi-dev libxrandr-dev \
  libxrender-dev libgl1-mesa-dev libudev-dev libasound2-dev \
  libpulse-dev libfreetype6-dev libfontconfig1-dev libssl-dev \
  libwayland-dev libxkbcommon-dev libvulkan-dev
python3 misc/build_products.py linux --jobs 2
```

Result: `Products/Godot-Engine-4.0.5-rc-debian-amd64.deb`.
Install it with `sudo apt install ./Products/*.deb`. The installer
includes the compiled editor executable and a launcher, not just an archive.

For Windows from **Debian** (optional cross compilation):

```sh
sudo apt install -y g++-mingw-w64-x86-64 gcc-mingw-w64-x86-64 nsis
python3 misc/build_products.py windows --jobs 2
```

Result: `Products/Godot-Engine-4.0.5-rc-windows-x86_64-setup.exe`.
On Windows itself, install Python, SCons, MSVC build tools and NSIS, then
run `python misc/build_products.py windows`.

For macOS, run on a **Mac** with Xcode tools, SCons, the Vulkan SDK and
`pkgbuild`:

```sh
python3 misc/build_products.py macos --jobs 2
```

Result: `Products/Godot-Engine-4.0.5-rc-macos-x86_64.pkg`.

A regular Debian setup **cannot compile macOS binaries** without a separately
configured osxcross/macOS SDK toolchain. The GitHub workflow
`.github/workflows/editor-installers.yml` builds the three platforms on
their respective runners and uploads their individual installers as artifacts.
Download the artifacts to combine them into one local `Products/` folder.

The packages are **unsigned** local builds. Windows SmartScreen and macOS
Gatekeeper may warn; production releases need platform-specific code signing
and notarization. The restored Git revision retains the original Godot branding.
The installable editor is the payload **inside** the setup installer.
