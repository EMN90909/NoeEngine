# NoeEngine repository-wide rename

The migration is a breaking rename of upstream API names, filenames, directories,
Godot-specific project extensions, C# assemblies/namespaces, build paths, and
platform packaging. The tracked tree of default branch `4.0` had **518 file
paths** containing `Godot` or `godot` when audited.

The rename is staged in `refactor/noeengine-complete-path-migration`, but that
path-only tree is **not build-validated and must not be mistaken for a complete
functional rename**.

The repo also contains `tools/rebrand_to_noeengine.py`, a migration tool
intended for a **fresh local checkout**:

```shell
python3 tools/rebrand_to_noeengine.py --dry-run
python3 tools/rebrand_to_noeengine.py --apply
git diff --stat
git grep -ni 'godot'  # legal upstream attribution is expected
```

The script renames tracked paths and visible identifiers, while preserving
copyright and licensing disclosures. This changes serialization formats,
native API identifiers, scripts, packages, plugins, and project compatibility.
Do not release without build, runtime, editor, and extension regression tests.

**Legal:** NoeEngine's independently owned additions may be proprietary.
Incorporated upstream source retains the MIT license in
[UPSTREAM-MIT-LICENSE.txt](UPSTREAM-MIT-LICENSE.txt), and component-specific
notices in [COPYRIGHT.txt](COPYRIGHT.txt).
