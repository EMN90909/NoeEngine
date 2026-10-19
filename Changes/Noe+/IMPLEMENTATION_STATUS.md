# Noe+ implementation status (incremental, not a release claim)

## Baseline
- Source baseline before this pass: `95e8aff5ccc1a897ff66c1539b71f011eefe0e01`.
- Base renderer: NoeEngine's Godot 4.0.5 RC-derived Forward+ (Vulkan) and Compatibility (OpenGL).
- Product display name follows the newer **NoeRender (Forward+)** convention. Legacy renderer IDs are unchanged.

## Implemented in source
- Native per-project `rendering/noe_plus/quality/preset` with Auto, Lite, Balanced, High, Ultra, Custom.
- Independent startup renderer choice and game graphics-quality profile. Custom preserves the project's individual renderer settings.
- Quality maps to 3D MSAA, positional shadow atlas, mesh LOD threshold, 3D scale and FSR 1.0 mode when Forward+ is selected.
- Compatibility profile gates out FSR and advanced Forward+-only settings; on GL it stays at native 3D scale.
- `rendering/noe_plus/quality/adaptive_3d_scaling` and `target_frame_ms` tune an optional game-only 3D scaling controller using smoothed **main-loop frame times**, threshold hysteresis and cooldown.
- 3D scaling changes only the root 3D render buffer, not the editor's UI rendering scale.
- New low-memory project renderer recommendation (under 8 GiB) uses Compatibility where compiled, without changing existing projects' renderer IDs.
- Standalone, GPU-independent C++ policy regression tests, Python source integration tests, and GitHub Actions workflow added.

## Evidence
- [Noe+ policy CI run 39273318342](https://github.com/EMN90909/NoeEngine/actions/runs/39273318342): **passed** on intermediate commit `a27e9b9` (C++ policy and Python integration tests).
- [Local preflight 39273318439](https://github.com/EMN90909/NoeEngine/actions/runs/39273318439): **passed**.
- [Windows build 39273318847](https://github.com/EMN90909/NoeEngine/actions/runs/39273318847) exposed macro pollution via `windows.h` in the shared startup translation unit. Fixed by removing that include from the Noe+ adapter. New native compilation still required to verify.
- [Linux editor build 39273318456](https://github.com/EMN90909/NoeEngine/actions/runs/39273318456): running when this record was created.

## Limitations and unimplemented acceptance gates
- Hardware **GPU** capabilities, Vulkan feature probing and crash-safe fallback are **not yet implemented**. RAM-based selection is a conservative heuristic, not a GPU driver test.
- No graphics-path profiler evidence or actual measured GPU frame-time feedback: the adaptive controller currently uses application frame time as a proxy. Do not advertise GPU-dynamic scaling.
- No visual quality comparison, low-end benchmark, renderer stress scene, memory leak audit, shader-cache optimization or new FSR algorithm yet.
- Real cross-platform native compile, runtime tests, Vulkan/OpenGL behavior and Noqeri game compatibility need documented evidence.
- These changes are **not** a complete implementation of the PDF, nor a validated v0.1.1 stable release.

## How to configure
Use **Project > Project Settings > Rendering > Noe Plus > Quality** (enable advanced settings when needed):
- `preset`: Auto (default), Lite, Balanced, High, Ultra, Custom.
- `adaptive_3d_scaling`: on/off, only effective on Forward+ with a non-Custom preset.
- `target_frame_ms`: 8 to 50 ms frame-budget target.
- Use **Custom** to preserve individually configured native Godot renderer values.
- Renderer identity is still controlled by native `rendering/renderer/rendering_method` and CLI switches.

## Next validation gate
Obtain Linux and Windows native build results; fix compiler failures; then launch real benchmark scenes and compare frame-time p50/p95/p99 and screenshots against the baseline on identical hardware. Never mark unchecked PDF acceptance items complete without that evidence.
