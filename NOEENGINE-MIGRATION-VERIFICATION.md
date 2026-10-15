# NoeEngine `main` migration verification

Verification date: 2026-10-15. Target repository: `EMN90909/noe`.

## Verified with GitHub's recursive tree API

- `main` HEAD at start of this verification: `f0931ad3d2f9a519a61c431bdaed456e56718970`.
- 9,874 tracked files.
- No tracked file path contains `godot` (case-insensitive).
- No tracked file path contains the erroneous `noeengineengine` doubled brand name.
- Android Java/Kotlin sources sampled in the corrected `org/noeengine` directories declare `org.noeengine` packages.
- `platform/android/java/lib/build.gradle` was inspected, as were the retained root and upstream licenses.
- The upstream MIT permission text is retained in `UPSTREAM-MIT-LICENSE.txt`, and the NoeEngine root license distinguishes proprietary additions from upstream MIT code.

## Not verified

The GitHub file and tree checks do not establish that *all internal references* have been migrated or that projects/builds work. Current checks **do not** constitute a local compile or runtime test.

The private repository could not be cloned to the local execution environment: `git ls-remote https://github.com/EMN90909/noe.git HEAD` failed with `Could not resolve host: github.com`. Therefore `python3 tools/rebrand_to_noeengine.py --dry-run` and `--apply` were **not executed in a full local checkout** in this verification. Since `main` already has zero Godot-named paths, a repeated `--apply` is not a substitute for native platform builds, editor boot tests and managed build tests.

## Required release gates

1. Run `python3 tools/rebrand_to_noeengine.py --dry-run` from a clean checkout of `main`; investigate any remaining text rewrites.
2. Run `git ls-files | grep -i godot` and verify that it returns no paths.
3. Validate Android Java/Kotlin/Gradle/JNI, Linux and Windows editor builds, managed C# assemblies and cross-platform export templates.
4. Run test projects, resource serialization, importers, plugins, and platform packaging.
5. Keep upstream copyright, MIT rights and third-party notices.
6. Change the repository default branch from `4.0` to `main` in GitHub repository settings; the connected GitHub tool does not offer repository-default-branch administration.

**Overall status:** path rename checks pass for `main`; source-wide semantic compatibility and functional builds remain unverified.
