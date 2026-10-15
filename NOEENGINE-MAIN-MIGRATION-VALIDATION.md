# NoeEngine main-branch migration validation (2026-10-15)

## Scope and results

The `main` branch already had **zero** tracked paths containing `godot` (case-insensitive) when this pass began. The default repository branch is `4.0`, but the requested commit target is `main`. A separate path-based audit of `4.0` found 518 Godot-named file paths.

A further inspection of the `main` tree found 56 malformed `noeengineengine` paths: three Linux desktop/metadata files and 53 Android Java/Kotlin files beneath `org/noeengineengine/`. This repair normalizes them to `noeengine`. It also updates Android platform text sources, Java/Kotlin symbols and package declarations, manifests, Gradle files, XML resources, and native JNI references toward NoeEngine naming. Required upstream MIT notices, copyright statements and third-party attribution have not been intentionally replaced.

**Static checks against the staged Git tree**:
- 9,872 tracked files before and after, with no remaining `godot` or `noeengineengine` path names.
- All 87 Android Java/Kotlin package-to-source-directory checks pass.
- All 17 referenced C# projects in three `.sln` files exist in the staged tree.
- Sample Android package declarations and Gradle namespaces reflect `NoeEngine`.
- The original MIT license and its copyright notices remain in `UPSTREAM-MIT-LICENSE.txt`.

## Reproduce in a clone

```sh
git checkout main
python3 tools/validate_noeengine_paths.py
python3 tools/rebrand_to_noeengine.py --dry-run
```

The validator is a *static smoke test*. A full engine checkout and locally available native build toolchains are required to compile/test the engine. GitHub Actions jobs in this repository were also previously observed failing before runner steps started. This audit **does not certify** builds, plugins, serialization migration, export templates, Android APK packaging, or removal of all text references in the rest of the engine. Any remaining original Godot reference in legal notices or upstream attributions must remain as required by license.

## Licensing

NoeEngine-only independently owned modifications may be distributed under proprietary terms to the extent legally authorized. Existing source derived from Godot retains MIT permissions; preserve `UPSTREAM-MIT-LICENSE.txt`, `COPYRIGHT.txt`, and all third-party notices.
