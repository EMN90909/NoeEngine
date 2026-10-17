# NoeEngine

**NoeEngine** is a game engine developed as a product of **[Noethric](https://noethric.xyz)**. It is a fork of the **Godot Engine 4.0.5 RC** codebase, with work in progress on its own tooling, packaging, and language integrations.

**Product release:** NoeEngine 0.1.0-dev (pre-release). **Engine compatibility base:** Godot 4.0.5 RC. The source is not yet a verified stable build.

## Features and programming languages

The inherited Godot editor supports its original project format, GDScript (`.gd`), and C++ through the engine and extension tooling. **Noqeri (`.nqr`) is now registered as a native ScriptLanguage and Script resource** through `modules/noqeri/`, alongside GDScript (`.gd`). The existing built-in Script editor opens `.nqr` source files, and supported Noqeri scene scripts can be attached to nodes and receive lifecycle calls such as `_ready()`.

**Current implementation boundary:** The native Noqeri scene bridge supports a **restricted, explicitly validated subset** of Noqeri (functions, variables, conditions, loops, Node calls). It converts this subset to the existing engine scripting VM. This is **not** full Noqeri compiler/ABI/stdlib/runtime parity. Unsupported Noqeri constructs fail instead of silently changing semantics. No GPL Noqeri compiler source or runtime is linked or bundled.

## Build and installation

From a Debian-family Linux machine, after installing dependencies in [BUILD_PRODUCTS.md](BUILD_PRODUCTS.md):

```bash
./build.sh linux 2
# With MinGW-w64 + NSIS installed, also:
./build.sh windows 2
```

Installer output belongs in `Products/`, using `NoeEngine-setup.deb`, `NoeEngine-setup.exe`, and (when built on macOS) `NoeEngine-setup.pkg`.

See [native Noqeri scripting](modules/noqeri/README.md), [product architecture](PRODUCT_ARCHITECTURE.md), and [security policy](SECURITY.md).

## Copyright, licensing, and upstream attribution

NoeEngine is a **product of Noethric**, built on the [Godot Engine](https://godotengine.org) project by its original authors and contributors. We acknowledge and preserve those contributions.

- **Godot-derived source:** Remains covered by its original **MIT license**, in [LICENSE.txt](LICENSE.txt), with the copyright and third-party notices in [COPYRIGHT.txt](COPYRIGHT.txt). These original rights cannot be revoked or replaced by proprietary terms.
- **Original Noethric additions:** Separately identified original files may be **proprietary** under [NOETHRIC-PROPRIETARY-LICENSE.txt](NOETHRIC-PROPRIETARY-LICENSE.txt). This applies only to code Noethric has the right to license that way, not to inherited Godot code or others' licensed work.
- **Noqeri compiler:** [Noqeri](https://github.com/EMN90909/Noqeri) is a separate project currently distributed under **GPL-3.0-only**. NoeEngine does not vendor its compiler. The GPL compiler is not redistributed or linked. The limited native scene bridge is independently written Noethric code that uses the engine's existing VM.
- Further details: [NOTICE.md](NOTICE.md), [LOGO_LICENSE.md](LOGO_LICENSE.md), and third-party notices under `thirdparty/`.

This product is independent of, and is not officially endorsed by, the Godot Engine project. Upstream file paths, classes, source headers, copyright notices, and `.godot` project compatibility remain unchanged where necessary.
