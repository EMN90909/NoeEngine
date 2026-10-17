# NoeEngine by Noethric

**NoeEngine** is a cross-platform 2D and 3D game-engine fork maintained by **Noethric**.
This repository is based on **Godot Engine 4.0.5 RC**. The core engine,
editor APIs and many source paths remain upstream-compatible. The Noethric
brand does **not** imply affiliation with or endorsement by the Godot project.

**Fork release:** NoeEngine 0.1.0-dev. **Upstream base:** Godot 4.0.5 RC.
This is an early-development fork, not a certified stable release.

## Build setup installers

On Debian or Ubuntu, use `./build.sh linux 2` to compile the engine editor
and produce `Products/NoeEngine-setup.deb`.

With a MinGW-w64 cross-compiler and NSIS installed, `./build.sh windows 2`
produces `Products/NoeEngine-setup.exe` on Linux. `./build.sh all 2`
builds both Linux and Windows on a Linux host. A macOS installer requires a
Mac and the Apple SDK: `./build.sh macos 2` creates
`Products/NoeEngine-setup.pkg`.

See [BUILD_PRODUCTS.md](BUILD_PRODUCTS.md) for the dependencies and
[PRODUCT_ARCHITECTURE.md](PRODUCT_ARCHITECTURE.md) for editor architecture.

## Origins, licensing, and attribution

NoeEngine incorporates work from the [Godot Engine](https://godotengine.org)
project. The original Godot source and copyright notices remain subject to
[LICENSE.txt](LICENSE.txt) and other included third-party licenses. Preserve
upstream notices in redistributed source and binaries. Original Godot logo
assets may also have separate attribution terms; see [LOGO_LICENSE.md](LOGO_LICENSE.md).

This fork is not yet a wholesale reimplementation or reskin of all inherited
native editor dialogs, icons, translations, and runtime strings. Internal
identifiers and file paths retain Godot naming where necessary to preserve
build compatibility.
