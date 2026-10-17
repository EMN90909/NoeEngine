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

## Noqeri editor integration (implemented, experimental)

`misc/noeengine_noqeri/addons/noeengine_noqeri` is an installable
Godot-compatible editor plugin supporting `.nqr` file recognition,
editing, syntax highlighting and externally executed Noqeri `check`.
A separate project installer is located at
`misc/noeengine_noqeri/install.py`. Addon files are staged into all
platform installer packages.

**Not implemented:** a native Godot `ScriptLanguage`, Noqeri scene
attachment, engine Node bindings, runtime lifecycle or game export
execution. In-process embedding of Noqeri's GPL-3.0-only compiler is
intentionally excluded from this proprietary-addon integration. See
`NOTICE.md` for licensing boundaries and source provenance.
