# NoeEngine migration audit

## Current default branch: `4.0`

Inspected the default-branch Git tree on 2026-10-15.

- **9,873 tracked files** before rebranding (the baseline includes six more files than the earlier 9,867-file audit).
- **518 tracked file paths** contain `godot` (case-insensitive), including files located beneath directories with Godot in the directory name.
- **77 directory paths** contain `godot` (case-insensitive).
- The deterministic proposed 518 rename destinations were checked against each other and the current tree: **zero collisions**.
- Source, text, build-file, packaging, and API references are not included in these 518 path matches.

## Migration execution status

The owner-triggered workflow `.github/workflows/noeengine-owner-migrate.yml` was corrected to target the actual default branch, `4.0`, and to check the file count, renamed paths, retained MIT notice, and staged Git changes before pushing.

The run [38738687035](https://github.com/EMN90909/noe/actions/runs/38738687035) failed **before any runner steps started** (GitHub reported an empty steps list and no assigned runner). It did **not** run `tools/rebrand_to_noeengine.py`, rename the 518 paths, or commit the migration. Check GitHub Actions account/billing/runner availability and the run's annotations before retrying.

The owner can alternatively run the checked-in migration tool against a **fresh local checkout of branch `4.0`**:

```bash
python3 tools/rebrand_to_noeengine.py --dry-run
python3 tools/rebrand_to_noeengine.py --apply
git add -A
git diff --cached --check
git ls-files | grep -i godot  # Must produce no file paths
```

**A zero-path match does not establish a successful functional rebrand.** Before merging or shipping, verify native and managed-code builds, editor startup, packaging/export pipelines, project serialization, GDExtension interoperability, C# assembly references, and plugin compatibility. Rename changes may intentionally break Godot-compatible project and API names and require migration tooling.

## Licensing

`LICENSE.txt` applies proprietary restrictions to independently owned, expressly proprietary NoeEngine additions. The included Godot-derived source retains its upstream MIT permissions and original notices in `UPSTREAM-MIT-LICENSE.txt`, `COPYRIGHT.txt`, and applicable third-party files.

**Status: not complete; do not claim that all references or build targets are migrated.**
