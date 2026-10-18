# NoeEngine Account Workspace (native editor)

## Implemented in this repository

The C++ editor and Project Manager both install `NoeAccountWorkspace` in their existing toolbars. On first use a non-blocking dialog offers **Sign in** or **Continue offline**. The top-right Account menu opens a native, keyboard-focusable settings window with left navigation: **Profile**, **Usage & limits**, **API & MCP**, **Extensions**, **Security**.

All panes reuse Godot 4.0 / NoeEngine UI controls, the built-in charcoal theme, `EDSCALE`, and translatable text. No external Electron/webview dependency is introduced.

- Profile: verified name/email after a successful HTTPS device authorization, plus a branded avatar fallback. Without a verified backend response it stays visibly offline.
- Usage: elapsed app session, cumulative local app time, and playtest runtime measured when the editor is running a scene. AI pie and bar charts support local/cloud call totals but remain **zero** until actual AI request instrumentation is connected. GPU rendering time and remote account quotas are **not yet measured**.
- API: validates and stores labels plus HTTPS endpoint URLs only. No API requests are made or secrets stored.
- MCP: lists saved remote HTTPS endpoints and enabled preferences. The in-app marketplace queries the official MCP Registry with `search=godot`, lists HTTPS remotes, and pre-fills a confirmation form. The Godot MCP Toolkit and official registry are linked. **No MCP transport, process, or tool execution is wired yet**. An enabled checkbox means locally configured, not connected. Untrusted packages are never launched automatically.
- Extensions: entry point for Godot-compatible per-project editor extensions.
- Security: current account status and logout. OAuth access tokens are memory-only and disappear on restart.

## Account backend needed for live sign-in

**The companion account website is implemented in [NoeEngine-web](https://github.com/EMN90909/NoeEngine-web)** with React, Supabase Auth, one-time device approval and a Node API. The website is not automatically live until deployed with its Supabase URL, anon key and server-side signing secret. In **Account > Security**, enter and save your real deployed HTTPS website URL. NoeEngine will use that setting for future browser sign-ins. `NOEENGINE_AUTH_URL` remains an optional process-level override.\n\n**An authentication service was not present in the NoeEngine repository.** The client does not invent users, trust a browser-provided email, or claim successful login without server verification. To activate live sign-in, deploy a real HTTPS account service and configure its base URL:

```sh
export NOEENGINE_AUTH_URL="https://accounts.example.com/noeengine"
./build.sh linux 2
```

The variable is read **at app startup**; it must also be present in the process environment when launching the packaged app, not just while compiling.

### HTTPS device authorization contract

1. `POST /device/start` body `{"client_name":"NoeEngine Desktop"}`, replies **HTTP 200**:

```json
{
  "device_code": "opaque-server-random-single-use-secret",
  "user_code": "ABCD-EFGH",
  "verification_uri": "https://accounts.example.com/noeengine/device",
  "interval": 5,
  "expires_in": 600
}
```

2. The app opens `verification_uri` in the browser. It must share the exact configured base origin and base path. The browser signs up/in through the account website and explicitly approves the device code.
3. The app calls `POST /device/poll` with `{"device_code":"..."}` every 5-30 seconds. Respond **202** or `{"status":"pending"}` while waiting, **429** or `{"status":"slow_down"}` to back off, and **200** with `{"access_token":"..."}` **only after server-side identity verification and device approval**.
4. The app calls `GET /me` with `Authorization: Bearer ...`; respond **200** with `{"email":"verified@example.com","name":"Display Name"}`. The UI marks the session verified **only** after this request succeeds.
5. Implement server-side TTL, one-time redemption, cryptographically secure device code generation, rate limiting, replay protection, HTTPS-only redirects, credential rotation, and explicit device authorization consent. The client currently forgets the session on app exit. A durable sign-in requires an OS keyring integration and refresh-token rotation, not plaintext token storage.

Other response shapes or an unconfigured backend leave the UI offline. The app does **not** grant remote MCP or API permissions simply because a device has authenticated.

## UI consistency and verification

Reference screens: `editor/editor_about.cpp`, `editor/project_manager.cpp`, `editor/editor_settings.cpp`; shared theme: `editor/editor_themes.cpp`. Controls reused: `MenuButton`, `AcceptDialog`, `ConfirmationDialog`, `VBoxContainer`, `HBoxContainer`, `TabContainer`, `ScrollContainer`, `Label`, `LineEdit`, `CheckBox` and `Button`.

The static regression tests run under `./build.sh check`, but **they are not a C++ compilation or visual UI test**. To verify on Linux:

```sh
git pull origin main
./build.sh check
./build.sh linux 2
```

Then manually test first-launch sign-in/offline, focus order, dialog rendering at 1366x768 and lower resolutions, chart empty states, the registry error path, API/MCP validation, closing/reopening the app, and device authorization against a deployed backend. Do not treat unauthenticated sample emails as accepted logins.

## Follow-on work required to complete the original full request

- Deploy a production user/account web service with registration, identity verification, device approval, refresh/revocation, and backend quota APIs.
- Use the system keyring for persistent sessions and refresh-token rotation.
- Instrument AI requests, GPU rendering and actual provider quotas; do not infer them from scene playtime.
- Implement MCP HTTP/SSE transport, OAuth authorization/permission prompts, tool allowlists, and testable enable/disable lifecycle before labeling a server **connected**.
- Build full extension installation, updates, signed manifests, and permission review UI.
- Validate a complete native C++ build, first-run screenshots and authenticated end-to-end journeys.

## Website connection and update checker (October 2026)

The companion React website repository is https://github.com/EMN90909/NoeEngine-web. The working Node/Express Render service is https://noeengine-web.onrender.com. The legacy static Render service has the shorter hostname but does not execute the device-authorization API.

Configure the native editor via Account > Security > Account website URL, using https://noeengine-web.onrender.com, or launch with NOEENGINE_AUTH_URL set to that exact HTTPS origin. This setting is used both for browser device approval and by the native update checker. The account backend remains disabled until the companion website has a dedicated Supabase project URL, a public publishable/anon key, and the schema migrations applied.

The new editor/noe_update_manager.{h,cpp} is included automatically by editor/SCsub. An Updates button appears in the Project Manager and editor top bars. The client checks GET /updates/latest?platform=linuxbsd&channel=stable (or windows/macos), compares published product versions against 0.1.0-dev and, when one is newer, shows an Update available action. It downloads only HTTPS public Supabase patch URLs, rejects nonallowlisted paths and oversized files, and verifies size plus SHA-256 before showing update notes and the staged patch's path.

**Do not mistake staging for installation.** This release does not execute downloaded patches, overwrite the installed engine, invoke a package manager or elevate privileges. The user installs the verified platform package separately and restarts. Full safe binary auto-update requires offline-pinned signing keys, rollback/recovery, and OS-native privileged helpers. Release publishing is gated by Supabase RLS and explicitly approved publisher accounts, but an unhashed/signed update manifest is not cryptographic proof of origin against a compromised trusted server.

Static regression tests: tests/python_build/test_noe_update_manager.py. Real C++ compilation, Linux window rendering, cross-platform installer behavior and update-download E2E remain unverified until the user runs ./build.sh linux 2 and tests with a published release.

## Implementation ownership and account service

**Native NoeEngine (this repository)** contains the top-right avatar, first-run welcome, Profile, Usage & limits charts, API & MCP management, extensions, and Security dialog. The account website is deliberately limited to browser signup/login/password recovery, one-time device-code approval, and publisher-only update uploads.

**Website and backend source:** https://github.com/EMN90909/NoeEngine-web

The website's Node API implements `POST /device/start`, `POST /device/poll`, and `GET /me`; Supabase Auth handles the browser identity, and dedicated PostgreSQL RPCs implement the short-lived, single-use device approval record. The desktop only shows a verified identity when `GET /me` returns a valid result. Tokens are memory-only, so a desktop restart currently requires signing in again. `NOEENGINE_AUTH_URL` overrides the website URL saved in Account → Security.

**Deployment still needs explicit connection:** The accessible Supabase projects are named Aroeno. No production NoeEngine Supabase project, deployed URL, or confirmed Render workspace has been supplied. Do not point the engine at a guessed `onrender.com` hostname and never install NoeEngine SQL in Aroeno. Follow the web repository's `DEPLOYMENT.md` only after a dedicated project and Render service are identified.

UI consistency measures: native `Button` controls, keyboard-focusable sidebar, default editor theme tokens, `EDSCALE` spacing, clamp-to-window account dialog, clear loading/error/offline states, and dynamic profile monogram. Static code inspection and source-level test coverage are not a substitute for a real Linux C++ compilation, visual screenshots, keyboard navigation, or authenticated integration testing.
