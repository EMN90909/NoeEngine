# NoeEngine

**NoeEngine** is a game engine developed as a product of **[Noethric](https://noethric.xyz)**. It is a fork of the **Godot Engine 4.0.5 RC** codebase, with work in progress on its own tooling, packaging, and language integrations.

**Product release:** NoeEngine 0.1.0-dev (pre-release). **Engine compatibility base:** Godot 4.0.5 RC. The source is not yet a verified stable build.

## Features and programming languages

The inherited Godot editor supports its original project format, GDScript (`.gd`), and C++ through the engine and extension tooling. NoeEngine additionally offers an **optional Noqeri (`.nqr`) editor integration** under `misc/noeengine_noqeri/`: edit, syntax-highlight, check, and inspect Noqeri programs using a separately installed Noqeri compiler.

**Important:** The Noqeri integration is editor/CLI tooling, not yet an implementation of Godot's native `ScriptLanguage` runtime. Noqeri scripts cannot yet be attached to scene nodes as replacements for GDScript. A Noqeri compiler executable is not embedded or redistributed in NoeEngine.

## Build and installation

From a Debian-family Linux machine, after installing dependencies in [BUILD_PRODUCTS.md](BUILD_PRODUCTS.md):

```bash
./build.sh linux 2
# With MinGW-w64 + NSIS installed, also:
./build.sh windows 2
```

Installer output belongs in `Products/`, using `NoeEngine-setup.deb`, `NoeEngine-setup.exe`, and (when built on macOS) `NoeEngine-setup.pkg`.

See [Noqeri integration](misc/noeengine_noqeri/README.md), [product architecture](PRODUCT_ARCHITECTURE.md), and [security policy](SECURITY.md).

## Copyright, licensing, and upstream attribution

NoeEngine is a **product of Noethric**, built on the [Godot Engine](https://godotengine.org) project by its original authors and contributors. We acknowledge and preserve those contributions.

- **Godot-derived source:** Remains covered by its original **MIT license**, in [LICENSE.txt](LICENSE.txt), with the copyright and third-party notices in [COPYRIGHT.txt](COPYRIGHT.txt). These original rights cannot be revoked or replaced by proprietary terms.
- **Original Noethric additions:** Separately identified original files may be **proprietary** under [NOETHRIC-PROPRIETARY-LICENSE.txt](NOETHRIC-PROPRIETARY-LICENSE.txt). This applies only to code Noethric has the right to license that way, not to inherited Godot code or others' licensed work.
- **Noqeri compiler:** [Noqeri](https://github.com/EMN90909/Noqeri) is a separate project currently distributed under **GPL-3.0-only**. NoeEngine does not vendor its compiler. Users install and invoke a compatible compiler separately; the GPL source is not relicensed by this project.
- Further details: [NOTICE.md](NOTICE.md), [LOGO_LICENSE.md](LOGO_LICENSE.md), and third-party notices under `thirdparty/`.

This product is independent of, and is not officially endorsed by, the Godot Engine project. Upstream file paths, classes, source headers, copyright notices, and `.godot` project compatibility remain unchanged where necessary.
