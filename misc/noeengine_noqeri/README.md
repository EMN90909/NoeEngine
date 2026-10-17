# Noqeri language integration for NoeEngine

**Publisher:** Noethric  
**Feature status:** editor + compiler CLI integration (experimental), not a
full Godot `ScriptLanguage` or scene-node execution runtime.

## What works

- `.nqr` files are imported into the Godot editor as `NoeNoqeriSource`
  resources. Import only reads source; no compilation or execution occurs.
- Select a `.nqr` file and choose **Project → Tools → Noqeri: Edit selected
  .nqr**, or use the **Noqeri** bottom panel's Open/New/Save controls.
- Edit with line numbers, highlighting for common Noqeri constructs and
  direct project-file saving.
- Choose a **separately installed trusted Noqeri compiler** in the Noqeri
  panel. The executable path is stored in local editor settings, not in
  the project, so shared projects cannot silently choose a binary.
- **Check** calls the actual compiler with `check <absolute-source.nqr>`
  (arguments are passed as an argv array, not through a shell).
- **Run*** requires explicit confirmation and invokes `run <source.nqr>`.
  The current Noqeri Stage-2 CLI does **not** provide `run`, so this
  experimental control is useful only with a compatible older runtime
  or a future Noqeri version that implements it.

## Install into a project

From a NoeEngine source checkout:

```bash
python3 misc/noeengine_noqeri/install.py /path/to/my-game-project
```

A Debian NoeEngine installation will place the same helper at
`/usr/share/noeengine/install_noqeri_addon.py`:

```bash
python3 /usr/share/noeengine/install_noqeri_addon.py /path/to/my-game-project
```

For Windows, the addon and helper are located inside the NoeEngine
installation folder; for macOS, in
`NoeEngine.app/Contents/Resources/`.

Then open the project in NoeEngine, navigate to **Project → Project
Settings → Plugins**, and enable **NoeEngine Noqeri**.

The installer refuses to overwrite an existing project's Noqeri addon.
No compiler is bundled, no binary is downloaded, and the plugin does not
enable itself automatically.

## Getting a compiler

Noqeri's canonical project is https://github.com/EMN90909/Noqeri
(at audited revision `145667eb0bd8bc1b1e04aaf34c7ab8a412bd2f8a`).
Current main uses self-hosted Noqeri source and needs an independently
trusted Stage-0 compiler plus Node.js 20+ to build a Stage-2 command.
The currently documented Stage-2 commands are:
`--version`, `check`, `lex-count`, `fingerprint`, and `selftest`.

Set an absolute compiler path using the Noqeri bottom panel's
**Compiler...** control. The optional `NOQERI_BIN` environment variable
is used when the editor has no saved path.

## Constraints and compatibility

This integration **does not** make `.nqr` scripts scene-attachable, provide
game-frame callbacks or automatic Godot Node bindings, or support runtime
script exports. That requires a supported ScriptLanguage/runtime bridge and
cross-platform ABI tests beyond this addon.

External compiler calls are synchronous in the current editor plugin.
Avoid long-running inputs: Godot's editor may pause until the process
finishes. Never run untrusted code or pick a compiler from an untrusted
project directory.

## Licensing

This adapter is original proprietary Noethric code under
`NOETHRIC-PROPRIETARY-LICENSE.txt` at the repository root.
The **Noqeri compiler is a separate GPL-3.0-only project** and is neither
copied into nor linked into this addon. The original Godot MIT rights
remain untouched. See `NOTICE.md` at the repository root.
