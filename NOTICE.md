# NoeEngine legal notices and attribution

**NoeEngine** is a product of **Noethric**. It is based on **Godot Engine
4.0.5 RC**. This fork is not affiliated with or endorsed by the Godot
Engine project.

## Godot Engine

Godot Engine © 2007–2014 Juan Linietsky and Ariel Manzur, and © 2014–present
Godot Engine contributors. Godot-origin code is licensed under the **MIT
License**. Preserve the full original license in `LICENSE.txt` and all
relevant copyright and third-party notices in `COPYRIGHT.txt`,
`thirdparty/`, and individual source headers. Noethric's proprietary
terms do **not** restrict this MIT-licensed upstream material.

Godot home: https://godotengine.org/
Godot source: https://github.com/godotengine/godot

The original Godot logo is separately licensed under CC BY 4.0, as explained
in `LOGO_LICENSE.md`. Do not infer any trademark or endorsement grant from
copyright licenses. Replace upstream branding assets before shipping a
distinctive commercial NoeEngine release.

## Noethric originals

© 2026 Noethric. Files explicitly marked with
`SPDX-License-Identifier: LicenseRef-Noethric-Proprietary` are original
Noethric proprietary additions, **to the extent Noethric owns the rights**.
See `NOETHRIC-PROPRIETARY-LICENSE.txt`. Existing MIT/GPL copyrights and
licenses are not superseded by such a notice.

## Noqeri (separate compiler)

Noqeri © 2026 Noethric and Noqeri contributors, GPL-3.0-only according to
the Noqeri source repository at commit
`145667eb0bd8bc1b1e04aaf34c7ab8a412bd2f8a`.

Compatibility review updated to source commit
`a577578ec48bff94bd66f133ad39349b926fedd9` (Noqeri 0.7.0-dev,
not released; language edition 2026, ABI version 2).

Noqeri compiler code is **not copied, statically linked, or bundled** by the
NoeEngine native scripting module. The independently written native module
implements a restricted game-script bridge using the inherited Godot VM,
not the full Noqeri compiler. A future release which bundles or links GPL-covered compiler
code must separately comply with the GPL, including source obligations, or
obtain appropriate additional relicensing rights from all rights holders.

Noqeri source: https://github.com/EMN90909/Noqeri
License: https://github.com/EMN90909/Noqeri/blob/main/LICENSE

## Third-party code

See `thirdparty/README.md`, `LICENSE.txt`, `COPYRIGHT.txt`,
individual source file notices, and relevant license inventories. A
proprietary distribution may contain independently licensed open-source
components whose rights must remain available to recipients.

Jamila-GD (NoeEngine project MCP integration)
------------------------------------------------
The local Jamila MCP helper is adapted from EMN90909/Jamila-GD.
That repository declares the Apache License, Version 2.0, in its package.
The VS Code extension itself is not embedded in NoeEngine. Only the project
MCP adapter and game-agent workflow were adapted into NoeEngine's native UI
and Python helper. Source: https://github.com/EMN90909/Jamila-GD
