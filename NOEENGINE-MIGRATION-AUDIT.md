# NoeEngine migration audit

Audited the `4.0` tree before merging the initial branding update.

- 9,867 tracked file entries
- 518 tracked **file paths** case-insensitively contain `godot` (including descendants of directories named Godot)
- Substantial remaining source-code string references are not counted by this path-only audit.
- `main` did not exist at audit time; `4.0` was the default branch.

## Work still required

1. Rebrand remaining visible names in the editor, application metadata, packaging and docs.
2. Rename filenames and directories together with their dependent import/build/package references.
3. Preserve backward compatibility for `project.godot`, GDExtension APIs, .NET assemblies, serialized resources, import paths, and plugin integrations until migration tools exist.
4. Preserve MIT license, upstream copyrights, and third-party license records.
5. Compile and test supported targets before declaring the migration complete.

**This audit does not certify that all names have been replaced or that the engine builds.**
