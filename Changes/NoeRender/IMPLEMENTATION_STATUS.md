# NoeRender implementation and verification status

## Scope and identity
- NoeRender is the sole user-facing graphics-family name. The original PDF remains archived under `Changes/Noe+/` for historical compatibility.
- Existing internal engine renderer IDs `forward_plus`, `mobile`, and `gl_compatibility` must not be renamed.
- Legacy `rendering/noe_plus/quality/preset` is read as a one-way migration default; new projects use `rendering/noe_render/quality/preset`.

## Implemented, awaiting complete GPU validation
1. Vulkan loader and real physical-device enumeration (vendor/device IDs, device name, Vulkan API version, local memory heaps and graphics/compute queue support). Apple MoltenVK enumeration deliberately defers to the native backend.
2. Preserves explicit CLI rendering selections, offers automatic OpenGL fallback for missing Vulkan loader/ICD or recoverable DisplayServer startup failure. Presentation support remains the backend's responsibility; fallback is not a guarantee against driver crashes.
3. Auto/Lite/Balanced/High/Ultra/Custom graphics policies and opt-in adaptive game-only 3D scaling. Mobile/GL are feature-gated.
4. Editor settings `interface/noe_render/low_resource_mode` and `interface/noe_render/focused_sleep_usec` use existing low-processor rendering mechanism without disabling project saves, debugger or input.
5. Opt-in `rendering/noe_render/debug/print_frame_diagnostics` prints local backend, frame-time, rendering counters and memory estimates.
6. Guarded stale descriptor cache pruning after viewport resize or scene teardown (no forced eviction of valid render RIDs). Shader-cache and clustered-light optimizations require profiler evidence before unsafe rewrites.
7. 3D rendered benchmark suite in `tests/noe_render_benchmark`, optional screenshot comparison using preapproved same-GPU/driver reference images and a manual GPU runner workflow.
8. `build.bat` invokes local `misc/setup_windows_build.ps1` to install missing dependencies with WinGet, MSVC/SDK, Python/SCons and NSIS.

## Evidence available
- Native policy tests on [run 39290001429](https://github.com/EMN90909/NoeEngine/actions/runs/39290001429): passed before full rename.
- Prior Windows/macOS installer builds on [run 39275387831](https://github.com/EMN90909/NoeEngine/actions/runs/39275387831): succeeded; Linux failed during a headless Noqeri editor smoke test, unrelated to graphics measurement.
- Later C++ compile and GPU tests must be checked before any claim of a stable full release.
- Benchmark scripts and screenshots must be **executed on physical GPUs** with fixed driver and hardware records. A CI machine running llvmpipe/Xvfb is not a physical GPU.
- **No measured shader-cache improvement, clustered-light speedup, memory reduction, GPU timestamp data or verified cross-platform fallback performance is claimed here.**

## Release blockers
- Actual Vulkan WSI and OpenGL fallback runtime QA on Intel/AMD/NVIDIA across Linux/Windows/macOS.
- Headless Noqeri editor SIGABRT investigation and regression closure.
- GPU profiler traces for lighting, shader compile, buffer lifecycle, GI, fog, reflections; backport only justified safe optimizations.
- Frozen cross-platform screenshots with per-device visual regression signoff and matched scene performance p50/p95/p99.
- Confirm Windows PowerShell bootstrap on a clean Windows installation with network permissions.
- Update checklist only for acceptance gates whose execution logs were verified.
