# NoeCore optimization: implementation and limitations

NoeCore is an **advisory, opt-in optimization layer**, not a replacement for the rendering or resource managers. Existing NoeRender Vulkan validation, adaptive resolution, editor low-resource mode, threaded filesystem scanning and Godot startup benchmarking are reused.

## Adaptive hardware detection
- Detects logical CPU threads and Linux host RAM, with a Vulkan candidate/estimated device-local heap from the existing probe. On other operating systems RAM is unknown in the early runtime path; existing editor hardware detection supports Windows and macOS.
- Recommends Low, Balanced, High or Ultra using explicit CPU, RAM, GPU, API capability signals. Unknown capability selects a conservative tier.
- The recommendation is displayed *only* when `performance/noecore/diagnostics/startup_benchmark` is enabled. It never changes a user's quality preset or renderer. The project creation UI displays an additional advisory tooltip but retains Auto as its default.
- GPU heap sizes are estimates, **not current available VRAM**. Presentation capability requires actual renderer startup, not this probe.

## Startup and editor responsiveness
- Reuses Engine startup benchmark timers. NoeCore diagnostics are disabled by default to avoid startup work and log noise.
- Removes an unnecessary duplicate per-frame texture import update; asset discovery already uses worker-thread scans.
- Does not globally defer required systems or disable user plugins because that would break projects.

## Memory
- `performance/noecore/memory/target_cache_budget_mb` configures a future shared cache target (0 = unset). **It does not currently impose a universal engine allocation limit.**
- `interface/noecore/preview_cache_budget_mb` controls the *editor preview cache* (0 = off). A bounded LRU pass only drops cache-held reference-counted thumbnails without outstanding external references; actual resources and scene contents are not unloaded.
- The C++ policy handles integer overflow and requires an over-budget state, an idle grace interval, and zero external references, no scene pin and no in-flight load before deeming any entry eviction eligible.
- Core allocator memory counters and `Performance` rendering/texture memory monitors already exist. Per-resource GPU residency tracking still requires renderer-specific instrumentation.

## Crash recovery and diagnostics
- `interface/noecore/session_recovery_enabled` controls a **local per-project active-session marker**, enabled by default. On unexpected exit the next editor session warns. Normal exit clears it.
- Editor layout writes use a temporary file followed by atomic rename; on failure the original layout remains intact.
- The existing editor layout contains reopened *saved scene tabs*. Unsaved scene edits are **not recovered**. Do not mistake the marker for a crash dump or full autosave facility.
- Diagnostics contain only hardware capacity counters and the recommendation. They never include project paths, source files, keys or model tokens; diagnostics are opt-in.
- No crash dumps are uploaded or sent to remote services.

## Optional features
- The existing SCons `module_<name>_enabled=no` switches (where offered) and export template `tools=no` should be used to build lean exports. NoeMixer and advanced cinematic/geometry systems must remain opt-in modules; no such modules are linked by this PR.
- Modularizing already-linked subsystems, true texture residency eviction, shader compilation prioritization, and full unsaved-scene recovery are **follow-on native engine projects**, not claimed as completed here.

## Checks
- `c++ -std=c++17 -Wall -Wextra -Werror -I . tests/noe_core_policy_test.cpp -o /tmp/noecore-test && /tmp/noecore-test`
- `python3 -m unittest discover -s tests/python_build -p 'test_noecore_regressions.py' -v`
- CI: `.github/workflows/noecore-policy.yml`.
- Full editor/export builds remain mandatory before merging major engine changes.
