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

**An authentication service was not present in the NoeEngine repository.** The client does not invent users, trust a browser-provided email, or claim successful login without server verification. To activate live sign-in, deploy a real HTTPS account service and configure its base URL:

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
