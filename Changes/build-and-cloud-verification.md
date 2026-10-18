# NoeEngine icon, build and backend verification

Date: 2026-10-18

## Changes committed to `main`

- Replaced `icon.png`, `icon_outlined.png`, `icon.svg`, `icon_outlined.svg` with the supplied orange cobra image. Both PNG files have the same Git blob (`9262679729ab28c2a5d18f309b9c3c1a0f1039e8`). Both SVG files have the same Git blob (`9e1adc509318b3090c74ee7449e70fc5c2510f3b`). SVG content embeds a PNG rendition of the supplied bitmap, not pure vector geometry.
- Updated `build.sh` for `linux`, `windows`, `macos`, `android`, `all`, `check` and `patch-tools`, including `--install-deps`.
- Added opt-in `misc/bootstrap_build_deps.py` for apt/Mac toolchain checks and checksum-verified Google's Android SDK download.
- Updated `misc/build_products.py` to create a Linux runnable AppDir and tar archive and copy a compiled Android editor APK; preserved Windows NSIS and native macOS PKG outputs.
- Updated preflight/tests/docs, including correcting stale native updater dialog wording and tests.

## Evidence observed this turn

1. **Icon integrity: verified.** GitHub's `main` tree lists all four requested files with the two SHA values above.
2. **Icon rendering: exercised locally outside the repository.** The optimized PNG reproduction was parsed with Pillow; an equivalent self-contained SVG opened with Python XML parsing and rendered to a 256×256 PNG via CairoSVG. This does **not** establish that every engine SVG renderer supports embedded PNGs.
3. **Build source contract: reviewed.** Nineteen source-level checks passed for build targets, dependency opt-in, checksum pinning, APK validation, Linux AppDir packaging, NSIS and pkgbuild usage, and CI references. These were static source checks, not actual package builds.
4. **Regression tests: committed but not executed against the full repository checkout.** The connected GitHub integration is for repository operations; no complete local source checkout or network clone was available in this environment.

## Production validation NOT passed

- **Real two-user Convex collaboration:** there is no accessible live Convex deployment/JWT configuration and no two test accounts. Existing Go/Convex code and source tests are not proof of a live two-user collaboration experience.
- **Supabase publication and notifications:** accessible projects are `Aroeno` and `Aroeno1`; there is no authorized dedicated NoeEngine Supabase project. No database migration, release insert, Realtime subscription or notification delivery was executed.
- **Full native C++ compile and restart:** NoeEngine's complete source tree and platform toolchains were not accessible together for compilation, and no signed update was deployed. The signed-patch helper from earlier work has unit tests, but a production end-to-end restart remains unverified.
- **Multi-platform binaries:** no genuine Windows `.exe`, macOS `.pkg`, Linux runnable binary or Android `.apk` was generated in this connected GitHub task. `build.sh` is now set up to do this on supported hosts.

## Exact next validation steps

```sh
git pull origin main
./build.sh check
./build.sh linux 2 --install-deps
./build.sh windows 2 --install-deps
./build.sh android 2 --install-deps
# On a Mac with Xcode installed:
./build.sh macos 2 --install-deps
```

Inspect `Products/` and `Products/build-*.log`. Install each output on the actual target OS/device, visually inspect the cobra icon, test the app's main screens and real restart flow. On a trusted NoeEngine Convex/Supabase deployment, separately run authorization-negative tests plus publisher→Supabase→Realtime→web/native notification tests.

A source commit is not equivalent to deploying a backend or successfully shipping a binary.
