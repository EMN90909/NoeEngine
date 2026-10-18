# Dependency and workflow security audit

Reviewed for the NoeEngine 0.1.0-dev fork baseline (Godot 4.0.5 RC).
This is an inventory and risk assessment, **not** a completed CVE scan.

## Vendored native libraries

The repository vendors a large `thirdparty/` tree. The authoritative
starting inventory is `thirdparty/README.md`. Examples recorded there:

| Package | In-tree version / reference | Risk / next check |
| --- | --- | --- |
| AMD FSR | 1.0.2 (2021) | Review upstream security and compatibility |
| astcenc | 4.4.0 (2023) | Review codec input parsing |
| Basis Universal | 1.16.4 (2023) | Review imported compressed texture safety |
| doctest | 2.4.11 (2023) | Build/test-only library |
| Embree | 3.13.5 (2022) | Review native geometry parsing and fixes |
| FreeType | 2.12.1 (2022) | Prioritize font parser updates |
| glslang | 11.12.0 (2022) | Prioritize shader compiler security review |

These dates and versions come from the checked-in dependency inventory.
They do not by themselves establish a vulnerability. Reconcile each vendored
component with upstream advisories and patched revisions, preserving
local patches in `thirdparty/` and licenses.

## Web developer toolchain

`platform/web/package.json` includes ESLint 7.x, JSDoc 3.x and their
plugins, managed by `package-lock.json`. Run `npm ci` and `npm audit`
in `platform/web` on a networked build agent. These are development
dependencies; classify exposure separately from shipped runtime code.

## CI external actions

Official actions used by the active builds (checkout, setup-python and
upload-artifact) are pinned to GitHub-verified commit SHAs. Legacy workflows
also use local reusable actions. Review these remaining external
integrations before re-enabling the full legacy matrix:

- `mymindstorm/setup-emsdk@v14` (Web toolchain, tag-based, not SHA pinned)
- Other vendored or legacy action configurations in `.github/actions/`
- Any installation from remote shell scripts or unverified package feeds

The optional `amm.../*-problem-matcher@master` references were removed
from the Linux and Windows legacy CI workflows.

## Remaining security work

1. Run OSV or an equivalent advisory scanner against vendored source,
   lockfiles, Python build tools, and GitHub Actions manifests.
2. Verify archive and download checksums for SDKs; pin release sources.
3. Add a reproducible software bill of materials (SBOM) to releases.
4. Verify native input fuzzing, project importers, and extension loading.
5. Review token permissions and secrets on every workflow.
6. Resolve Actions runner pre-step failures before relying on CI attestations.

No credential exposure or specific exploitable CVE is asserted without
supporting evidence.

## Native Noqeri integration licensing and provenance

The current compatibility target is Noqeri **0.7.0-dev**, language edition
**2026**, ABI metadata **v2**. The upstream game SDK currently has source
under `Lib/game/` but is not a qualified native game toolchain, per its
`Doc/GAME_DEV_READINESS.md` and `Doc/GAME_PLATFORM_MATRIX.md`.

The in-engine `modules/noqeri` code implements an original Noethric
native `ScriptLanguage` wrapper and a limited scene-script syntax bridge
to the existing Godot VM. It was designed following a review of
`EMN90909/Noqeri` at commit
`145667eb0bd8bc1b1e04aaf34c7ab8a412bd2f8a`.

That separate Noqeri compiler and runtime remain **GPL-3.0-only**.
No Noqeri compiler source, binaries, NIR runtime, standard library,
or GPL implementation has been copied or linked into the engine.
The current source reference is commit
`a577578ec48bff94bd66f133ad39349b926fedd9`.
The game-script subset bridge is **not** interchangeable with the
complete Noqeri compiler. Native runtime/API parity remains a separate
engineering and licensing review requirement.
