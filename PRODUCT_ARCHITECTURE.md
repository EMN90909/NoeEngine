# NoeEngine product identity and editor architecture

## Product identity (decision)

- Product: **NoeEngine**
- Publisher: **Noethric**
- Public display name: **NoeEngine by Noethric**
- Fork release: **0.1.0-dev**
- Engine compatibility baseline: **Godot 4.0.5 RC**
- Source language/build: C++ / SCons
- Current editor: inherited native Godot editor, with limited display branding.
- Installer outputs: `Products/NoeEngine-setup.deb`,
  `Products/NoeEngine-setup.exe`, and `Products/NoeEngine-setup.pkg`.

The engine's internal `godot.*` SCons filenames, `res://` resource paths,
GDScript APIs, `.godot` project files, generated headers, and third-party
upstream attributions are **intentionally unchanged** for compatibility. A
product rename must not be implemented as an indiscriminate search/replace.

## Proposed editor architecture (not yet implemented)

Three user-selectable workspaces on one native editor runtime:

1. **Simple:** smaller Library/Objects rail, Scene hierarchy, concise Inspector,
   large central 2D/3D viewport, primary Create/Move/Play actions.
2. **Mid:** adds graphics, terrain, geometry, profiler, materials and accessible
   command search without sacrificing the viewport.
3. **Extreme:** original native Godot editor layout, menus, inspectors, dock
   management, scripting, debugging, scene tools, and shortcuts.

Implementation approach: an editor plugin layer first; native C++ changes only
where addon interfaces cannot provide the requested behavior. Store the chosen
mode per-user in editor settings. Restore/reveal native docks in Extreme mode.
Never duplicate node editing rules or bypass UndoRedo. For each layout:
keyboard navigation, visible focus, truncation handling, 1366x768 checks,
larger-monitor checks, and project reopening tests are required.

## What exists now versus future work

The 0.1.0-dev source at this revision does **not** have the custom editor
suite that was previously present in later commits. The above workspace
design is a technical specification only. This commit does not claim that
Simple/Mid/Extreme modes or Jamila are running.

## Release gates

1. Run `bash misc/ci_check.sh` locally.
2. Compile an actual native Linux editor via `./build.sh linux 2`.
3. Test version/help, launch editor with a display, create/save/reopen a scene.
4. Validate the Debian installer install/uninstall on Debian and antiX.
5. Compile Windows and macOS builds and smoke-test on those systems.
6. Verify third-party license and copyright notices are included.
7. Sign/notarize release artifacts before public distribution.

A locally passing packaging test alone is not a release gate for the C++
engine. See `BUILD_PRODUCTS.md` and `DEPENDENCY_AUDIT.md`.

## Native Noqeri ScriptLanguage integration (experimental)

`modules/noqeri/` defines `NoqeriScript`, `NoqeriLanguage`, and the `.nqr`
resource loader/saver. It registers with `ScriptServer` alongside GDScript.
The built-in Script editor handles Noqeri source, code highlighting and source
saving, and compatible Noqeri scripts may be attached directly to nodes.

The first engine runtime uses a deliberate subset-to-GDScript execution
bridge. That supports familiar lifecycle callbacks and engine Node methods,
but does **not** implement the complete Noqeri standalone language or its
native backend, fixed-width systems types, FFI, imports, atomics, memory safety
model, standard library, or NIR. Unsupported semantics fail closed. Script
instance dispatch currently uses the inherited Godot VM and debugging frames
may carry translated line numbers. A future full runtime must establish
ABI/host-services parity and license compliance before replacing the bridge.

See `modules/noqeri/README.md` and `tests/noqeri_scene/`.

## Noqeri source compatibility tracking

Native scene scripting tracks Noqeri **0.7.0-dev** (unreleased), edition 2026,
ABI v2 source reference at commit `a577578ec48bff94bd66f133ad39349b926fedd9`.
`modules/noqeri/compatibility.json` records the boundary. This does not
embed the self-hosted Noqeri Stage-2 compiler or qualify the upstream game SDK.
