# Jamila game-building agent in NoeEngine

Jamila is **a native, toggleable panel** on the editor's top-right toolbar. Opening it overlays
the Inspector and right-side dock without moving or destroying editor dock layouts.
It uses a Python stdlib-only agent and a **local stdio JSON-RPC MCP server**, adapted
from [Jamila-GD](https://github.com/EMN90909/Jamila-GD/tree/Main/integrations/noeengine).
This is **not** the whole Jamila-GD VS Code extension embedded inside Godot.

## What really happens

1. Open a game project with \`project.godot\`, click **Jamila** and enter an HTTPS
   OpenAI-compatible chat endpoint, model name, API key, and a game request.
2. Click **Generate plan for review**. The local MCP reads project name, asset
   names, and up to four small scripts/scenes. Only those samples, the inventory
   and prompt are sent to the selected model. Don't put secrets in game files.
3. Jamila proposes up to eight \`.gd\`, \`.nqr\`, \`.tscn\`, \`.tres\`, or shader
   files. Inspect **every proposed file's full text** and whether it overwrites
   an existing file.
4. Click **Approve changes, build and validate**. Only then can the local MCP
   write within the selected project. Jamila refuses stale plans if any target
   file changed since planning. The engine runs a real headless editor import
   check, followed by a bounded 120-frame headless scene playtest when the
   approved plan changed a scene. It reports errors and keeps export disabled
   when validation fails.
5. With a **Project → Export** preset configured and matching export templates
   installed, choose the exact preset name. **Export validated game (.pck)**
   creates a game-data pack; **Build playable release** uses
   \`--export-release\` to create an executable/package under \`builds/\`.
   That export must be manually tested on its actual target OS/device.

## Security model and limitations

- Local service bound to \`127.0.0.1\` with a random per-session bearer.
  Browser-origin requests are blocked; no public listener or cloud secret storage.
- Read/write paths deny absolute paths, hidden directories, symlinks,
  directory traversal, and non-game extensions. No arbitrary shell tool.
- Mutations, scene execution, and export require the agent's approval token.
  Plans expire after ten minutes. API keys are kept in editor memory only.
- Runtime smoke tests are not a substitute for interactive graphics, physics,
  multi-platform export verification, security review or human QA.
- No unrestricted filesystem/process control, VS Code terminal integration, or
  arbitrary external MCP transport is bundled. The website's MCP endpoint
  pairing is a separate capability; it is **not** automatically available to Jamila.
- The model is asked to follow constraints, but **model output is untrusted**.
  Review generated scripts before approval: scene execution runs game code
  with your desktop user's permissions.
- On machines with limited RAM the native editor can use compact visuals.
  No browser or Electron sidecar is required.

## Development and validation

Run from the repository root:

\`\`\`bash
python3 -m py_compile tools/jamila/jamila_agent.py tools/jamila/noe_mcp_server.py
python3 -m unittest discover -s tools/jamila -p 'test_*.py' -v
./build.sh check
\`\`\`

These Python tests use a **fake engine executable** and verify MCP protocol,
permission checks, proposal approval, scene-run subprocess handling, and release
output wiring. They do **not** prove that native C++ compiles, real Godot scenes
are valid, or cross-platform export templates work. A full NoeEngine native
build, editor UI inspection and a real sample game end-to-end test remain
necessary before calling the integration production ready.

For packaged builds, \`misc/build_products.py\` stages
\`tools/jamila/jamila_agent.py\` and \`tools/jamila/noe_mcp_server.py\` alongside
the editor. Python 3 must be available on the user machine. The actual
\`--headless\` and \`--export-release\` commands run against that installed
NoeEngine executable, not a generic Godot binary.
