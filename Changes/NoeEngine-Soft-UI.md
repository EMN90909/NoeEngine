# NoeEngine Soft UI: source patch and release requirements

## What changed

- `NoeEngine Soft` is the **new default for fresh editor settings**. Existing installations retain their saved theme until changed manually.
- `NoeEngine Dark` remains available as the original lightweight classic interface, preserving its original colors, contrast, sharp edges, menu and dock layout.
- The Soft preset uses warm orange focus/accent, graphite surfaces, 6-unit native rounded button corners, larger horizontal click areas and a stronger hover state.
- Both presets are implemented using the existing native `StyleBoxFlat` renderer. No webview, blur compositor, new GPU shaders, framework dependencies or additional background processes are introduced.
- Editor user settings: **Editor → Editor Settings → Interface → Theme → Preset**. Select `NoeEngine Soft` or `NoeEngine Dark`.

## Source patch

`Changes/NoeEngine-Soft-UI-source.patch` contains a standard unified `git apply` patch against the main-branch baseline at the time of creation. Only the engine theme and settings code is touched.

This **source patch is not an installable `.noepatch`**. The installed editor's UI is compiled into the executable, so source changes alone cannot update a running binary.

## Required validation before release

1. Verify `git apply --check Changes/NoeEngine-Soft-UI-source.patch` on an **unmodified main checkout**. Don't reapply to this branch, which already contains the changes.
2. Trigger the Linux editor baseline GitHub Actions workflow; compile the editor and run native Noqeri tests.
3. Start the built binary and compare appearance and operation on a weak-GPU Linux system and a modern device, including keyboard focus, inspector, scene tree, filesystem, 2D and 3D viewports.
4. Check the saved legacy theme remains unchanged when chosen, including upon restart. Select Soft and confirm the preset persists.
5. Use `./build.sh patch-tools` to build the offline signer and updater; generate and offline-sign a `.noepatch` for an **exact installed binary baseline**, following `docs/SIGNED_PATCH_UPDATES.md`.
6. Test the updater on a disposable install: validate signature and hashes, change detection, backups, rollback on failure, and clean restart.
7. In Supabase project Aroeno1 use storage bucket `noeengine-patches` and table `noe_releases` **only after** the signed artifact passes testing and an approved release publisher uploads it via the NoeEngine website `/releases`.

Do not publish a source diff as an installable binary patch or create a `noe_releases` row before a real signed package exists.

## Auth/backend ownership

The native editor already uses the Supabase-backed `NoeEngine-web` device authorization and `/updates/latest` release feed. The blank approval webpage must be investigated in the website deployment. Supabase project Aroeno1 contains the device-link tables and release records. This theme patch does not modify authentication secrets or desktop tokens.
