# Noe+ / NoeShader Implementation Checklist

> Status: PARTIALLY IMPLEMENTED. See IMPLEMENTATION_STATUS.md for validated source changes and outstanding native renderer tests.
> Repository baseline verified: NoeEngine 0.1.0-dev, Godot 4.0.5 RC fork. Target: v0.1.1.
> Owners: NoeEngine engineering team. Check items only after evidence is captured.

## 0. Baseline and safety gates
- [ ] Record immutable baseline commit, upstream Godot version, build options, OS, graphics driver, GPU, RAM, and test scene.
- [ ] Review changes already present in `editor/`, `modules/noqeri/`, and the NoeEngine account/update subsystems; do not overwrite unrelated work.
- [ ] Complete `./build.sh check` and `./build.sh linux 2`; capture actual compiler output and exit codes.
- [ ] Run editor GUI smoke test on Linux/X11 and launch headless test project; archive logs.
- [ ] Create representative 2D, small 3D, lighting stress, shader-compile, and low-end benchmark scenes.
- [ ] Establish upstream Godot baseline on identical scenes; capture cold/warm startup, frame time p50/p95/p99, memory, GPU memory where available, draw calls, and shader stutter.
- [ ] Review source-compatibility and applicable MIT/third-party attribution before edits.
- [ ] Capture baseline visual screenshots; maintain golden screenshots and test-scene project assets.
- [ ] Document explicit limitations of the Godot 4.0.5 RC foundation and select any upstream backports independently.

## 1. Name, identity, compatibility
- [ ] Define public brand: Noe+ is the adaptive rendering family; NoeShader is the Forward+ display name; Noe+ Lite is the Compatibility path.
- [ ] Locate every user-facing renderer label in editor/project manager and documentation before patching.
- [ ] Change Forward+ **display text only** to NoeShader; avoid globally renaming `forward_plus`, `gl_compatibility`, rendering method IDs, serialized project settings, shader macros, or upstream source filenames.
- [ ] Maintain old project files and CLI renderer arguments; document aliases and migration tests.
- [ ] Ensure project creation, project reopen, editor settings and export templates preserve renderer selection.

## 2. Renderer choice and GPU capability detection
- [ ] Audit renderer/driver selection in `main/main.cpp`, project manager and display-server initialization.
- [ ] Probe Vulkan device/feature availability safely and test OpenGL compatibility before selecting a backend.
- [ ] Design a capabilities struct (API/version, driver/vendor/device IDs when exposed, memory budget estimates, compute, texture limits, render features); do not infer support from GPU marketing names alone.
- [ ] Respect explicit project/CLI/user renderer overrides unless they would crash; show actionable error/fallback messaging.
- [ ] Select a **startup backend**; never hot-swap Vulkan and OpenGL in a live frame.
- [ ] Add conservative safe fallback from unsupported Vulkan to Compatibility in a supported startup recovery path, with no crash/restart loop.
- [ ] Make hardware classification a heuristic with overridable defaults and telemetry-free local storage.
- [ ] Test older Intel integrated graphics and software-renderer scenarios; distinguish unsupported devices from poor performance.
- [ ] Test Linux Mesa, Windows drivers and macOS/MoltenVK independently before promising support.

## 3. Noe+ quality manager
- [x] Implement Lite, Balanced, High and Ultra quality policy values; add Auto and Custom modes (source-level, native runtime build pending).
- [x] Keep rendering backend independent from graphics quality preset.
- [ ] Map presets to explicit engine settings (3D scale, shadows, anti-aliasing, screen-space effects, GI, reflections, fog, anisotropy, texture budgets, particle counts).
- [ ] Gate each option against renderer feature support; never enable Forward+ only effects on Compatibility.
- [ ] Permit manual override and per-project persistence; support reset to auto-detected defaults.
- [x] Add hysteresis and cooldown to opt-in adaptive 3D render-scale changes; restrict to the game root 3D viewport (GPU runtime behavior still unverified).
- [ ] Prefer stable frame time and measurable quality over a blanket FPS guarantee.
- [ ] Ensure screenshots, editor viewport and exported game apply settings consistently where appropriate.

## 4. Forward+ / NoeShader improvements
- [ ] Profile `servers/rendering/renderer_rd/forward_clustered/render_forward_clustered.cpp` and related headers/shaders before optimizing.
- [ ] Review clustered light list construction, light count limits, shadow atlas allocations and GPU synchronization.
- [ ] Inspect render buffer creation paths for unnecessary MSAA/specular/roughness/velocity textures; modify only with per-pass lifetime evidence.
- [ ] Audit overdraw, material shader variants, transparent materials, depth prepass and draw-call batching.
- [ ] Profile SDFGI, VoxelGI, volumetric fog and reflection probes across presets; maintain feature parity for supported scenes.
- [ ] Test visual correctness under HDR/SDR, MSAA, camera motion, resize, minimized windows and multiple viewports.
- [ ] Keep rendering APIs, GDExtension behavior and Noqeri scene integration stable.

## 5. Upscaling, shader cache, memory
- [ ] Examine `servers/rendering/renderer_rd/effects/fsr.cpp`; existing FSR infrastructure is confirmed, but integration and device support require runtime testing.
- [ ] Review `servers/rendering/renderer_rd/storage_rd/render_scene_buffers_rd.cpp` for scaling and sharpness behavior.
- [ ] Evaluate FSR path and supported fallback; assess visual quality, aliasing and ghosting in motion.
- [ ] Review shader cache setup in `servers/rendering/renderer_rd/renderer_compositor_rd.cpp`; cache code exists, but performance is unmeasured.
- [ ] Benchmark shader warmup, cold cache, cache corruption, invalidation, driver updates and disk usage.
- [ ] Track GPU/CPU allocations under scene reload and changing presets; verify no leaked RIDs or stale descriptors.
- [ ] Protect low-RAM systems from unbounded shader cache and transient allocation growth.

## 6. Noe+ Lite / editor low-resource mode
- [ ] Verify Compatibility pipeline at `drivers/gles3/rasterizer_gles3.cpp` and related GLES3 features.
- [ ] Keep lightweight 2D and small 3D projects usable on older supported OpenGL hardware.
- [ ] Introduce opt-in editor low-resource mode: minimal viewport update cadence, background tool throttling, optional preview suppression and smaller caches.
- [ ] Do not disable project saves, errors, input, debugger controls, Noqeri editing, accessibility or required account/security functions.
- [ ] Profile slow storage, 6 GB RAM and dual-core CPUs; do not claim Celeron 1005M works until the editor launches and is measured.
- [ ] Check 1366x768 layout, panel clipping, keyboard navigation and text scaling.

## 7. Observability and tests
- [ ] Add optional local performance HUD with backend, GPU/driver, quality tier, render scale, frame time, draws, triangles and memory estimates.
- [ ] Add automated preset serialization, override precedence, feature-gating and fallback unit tests.
- [ ] Add integration tests for project creation/reopen and renderer selection.
- [ ] Run screenshot regression checks across Lite/High and forward/compatibility paths.
- [ ] Test crash/recovery: missing Vulkan loader, unsupported extensions, corrupted shader cache, low-memory allocation failure and driver errors.
- [ ] Run graphics stress scenes over extended sessions to check leaks and stutter.
- [ ] Compare before/after benchmarks at identical resolutions and settings; record regressions even when FPS improves.
- [ ] Perform Linux, Windows and macOS smoke checks or mark untested platforms explicitly.

## 8. Packaging, docs and release
- [ ] Update NoeEngine docs, renderer picker description, CLI help and migration notes without rewriting Godot attribution.
- [ ] Include default Auto selection and manual override help in editor.
- [ ] Preserve installer behavior, third-party notices and engine ABI promises.
- [ ] Capture source SHA, test logs, screenshots and reproducible benchmark tables in release evidence.
- [ ] Require actual compiled editor, driver-specific runtime tests and regression sign-off before calling v0.1.1 stable.

## Explicitly deferred past v0.1.1
- [ ] Evaluate modern Godot renderer subsystem backports after compatibility research.
- [ ] Research temporal upscaling and optional hardware ray tracing; do not claim inherited FSR is FSR 2/3.
- [ ] Consider virtualized geometry, bespoke GI or independent rendering backend only with a cost-benefit study.
- [ ] Keep NoeEngine Cloud paid remote GPU rendering jobs as a separate product/service; do not couple it to local real-time rendering.

## Definition of done
Noe+ v0.1.1 is done only when both NoeShader and Compatibility selection are correct, presets and fallback are tested on supported hardware, editor/game builds pass, measurable performance results are published, rendering comparisons are visually reviewed, migration tests pass, and all unresolved limitations are explicitly documented.
