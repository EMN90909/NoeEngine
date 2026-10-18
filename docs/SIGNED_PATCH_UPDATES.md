# NoeEngine signed patch updates (Linux pilot)

## Release architecture

1. A release publisher builds and tests NoeEngine locally. Produce a **commit-style patch** with a `manifest.json` listing changed files, their original and replacement SHA-256 digests, and a semantic target version. Operations are `add`, `replace` or `delete`.
2. Sign exactly those manifest bytes using a private Ed25519 key held **offline**. The `manifest.sig` is a raw 64-byte signature; the `files/` folder contains only changed files. Package everything as a ZIP named `NoeEngine-X.Y.Z.noepatch`.
3. The authorized publisher uploads the package at NoeEngine-web `/releases` to Supabase Storage bucket `noeengine-patches`. The public release catalog `noe_releases` records version/platform/channel, SHA-256, size and human release notes. It transitions from draft to published under RLS.
4. The website receives Supabase Realtime published-row updates and shows an announcement with a download link. A two-minute web polling fallback catches missed events. The native editor checks the website release feed eight seconds after launch, again after each five hours of continued uptime, and whenever the user clicks Updates.
5. After the user clicks **Download update**, NoeEngine downloads into a private cache directory, validates size/SHA-256 and URL, and offers **Install and restart** only if a trusted local patch helper and public key have been configured.
6. On explicit approval, NoeEngine starts the local updater, exits, then the helper waits for the editor PID to stop. It verifies the package signature and every changed file's before/after checksums, rejects unsafe paths and symlinks, backs up originals, applies changes, records a receipt, and attempts to restart the executable.
7. On the next startup the editor reads `last_update.json`, records the installed version in `noeengine_account.cfg`, and displays **What's new** with **Continue**.

## Build and sign offline

Build the local tools on Linux, without starting the full C++ build:

```sh
./build.sh patch-tools
# One-time key generation on a trusted offline machine:
Products/noe-patch-sign -keygen /secure/offline/noe-release.key
# Record the printed base64 public key; do NOT upload the private file.
```

Example `manifest.json`:

```json
{
  "version": "0.1.1",
  "base_version": "0.1.0",
  "notes": "Faster scene loading and collaboration fixes",
  "changes": [
    {
      "path": "NoeEngine",
      "op": "replace",
      "before": "<SHA-256-OF-OLD-BINARY>",
      "after": "<SHA-256-OF-NEW-BINARY>"
    }
  ]
}
```

Place new files under `patch-files/` mirroring their relative installation paths, then:

```sh
Products/noe-patch-sign -manifest manifest.json -files patch-files \
  -key /secure/offline/noe-release.key -out NoeEngine-0.1.1.noepatch
```

For initial Linux installation bundle a **prebuilt, trusted** `Products/noe-updater` along with the engine. The updater binary must not be downloaded from a release or extracted from a patch to execute itself. Configure:

```sh
export NOEENGINE_AUTH_URL=https://YOUR_REAL_NOEENGINE_WEBSITE
export NOEENGINE_PATCH_HELPER=/absolute/path/to/preinstalled/noe-updater
export NOEENGINE_UPDATE_PUBKEY=BASE64_PUBLIC_KEY_PRINTED_DURING_OFFLINE_KEYGEN
```

The native client now has the verified deployed default `https://noeengine-web.onrender.com`, and still allows environment-variable and Account → Security overrides. Do not hardcode database credentials or Convex deployment keys in the native editor. Published releases cannot be installed if the helper or signing key is missing. The private key remains offline.

## Verified locally

- The offline signer built using Go 1.23-compatible source and produced a signed `.noepatch` fixture.
- The helper verified the signature, applied a file change, and wrote a version/changelog receipt.
- Go tests passed for mismatched baseline hashes, malicious file paths, incorrect signing key and symlink escapes.
- A source-equivalent Node release-selection test passed version ordering, draft filtering, hash/path rejection, and download metadata.
- **Not verified:** actual full native C++ build, GUI screenshot/UX, deployed Supabase database and event subscription, privileged/system-wide package installation, Windows/macOS updater execution, disk-full/power-loss crash recovery, or real restart of the NoeEngine binary. This is a Linux writable-directory pilot, not a crash-atomic or multi-platform update system.

## Release safety

- **Do not** use `git apply` or run a script from an uploaded bundle; all changed files must be explicitly listed and signed.
- Patch publication should be performed only by the publisher role; Supabase RLS protects insert/update, and public users can only read published releases.
- File-level rollback is best-effort on ordinary errors. Abrupt power loss during a multi-file update still needs a journaling or directory-swap design before production rollout.
- A web release announcement is **not** an OS push message to offline users. Browser delivery requires an open site; native checks happen periodically.


## Additional release-verification gates (2026-10-18)

The signer rejects invalid semantic versions, invalid base or target hashes, and inconsistent add/delete metadata before producing a usable package. A failed signing operation removes its incomplete output. The updater preserves executable permissions for replaced files and gives newly added files non-executable permissions by default.

`tools/noe-updater/end_to_end_test.go` exercises the actual offline signer executable from the updater tests, including:
- Sign a fixture, install it in a temporary directory, and validate the receipt
- Reject a mismatched version and the wrong Ed25519 public key
- Reject changed payload bytes after signing
- Preserve the prior executable file mode; prevent new files from becoming executable

`tools/noe-patch-sign/main_test.go` covers invalid versions, paths, checksum conflicts and incomplete-archive cleanup. These tests run as part of `./build.sh patch-tools`.

**Current verification status:** These source tests and a GitHub Actions workflow are committed, but the current GitHub Actions runs report **failure without accessible job logs**. A new successful Go test run has not been independently observed here, and full native C++ compilation is not confirmed. Run the commands below on a complete checkout and do not publish a patch until they succeed:

```sh
./build.sh patch-tools
./build.sh check
./build.sh linux 2 --tests
```

The first two require Go and Python; the final command requires the native toolchain and Godot/NoeEngine build dependencies. Production updates additionally require a reviewed signing-key custody process, disk-failure rollback strategy and a verified website release catalog.
