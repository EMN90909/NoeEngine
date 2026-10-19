# NoeProfiler: frame-time investigation (incremental integration)

NoeProfiler extends the **existing engine debugger** instead of replacing it.
The **Profiler** panel now exposes a configurable spike threshold, a **Next Spike**
button and a per-frame attribution strip. Scrub the graph or use Next Spike to
inspect a 45 ms frame. Script functions are already enumerated in its CPU tree.

## What can be measured today

| Area | Sources | State |
| --- | --- | --- |
| Overview | FPS, CPU frame/process/physics timings, frame spikes and rolling monitor history | Available using existing Debugger > Monitors and Profiler |
| CPU | Per-function script total/self time, process/physics measurements, server profiler categories | Available where the existing remote profiler supplies data; thread-level CPU scheduling not traced |
| GPU | Rendering passes and CPU/GPU timing markers in Visual Profiler; draws and memory counters in Monitors | Existing backend dependent instrumentation; **not** guaranteed on OpenGL |
| Memory | Static allocator peaks, object/resource counts, renderer/texture/buffer estimates | Available as aggregate monitors; per-allocation lifecycle tracking not added |
| Physics | 2D/3D active bodies, collision pairs and islands, frame physics time | Existing counters; per-query tracing not yet added |
| Assets | Existing import/preview caching and startup timings | Global import stalls/dependency timeline not yet instrumented |

### Spike interpretation

At a **45 ms** frame with **21 ms process CPU**, **13 ms physics CPU**
and **11 ms scripts inside process**, NoeProfiler displays a frame spike,
the largest *measured* CPU category, and **at least 11 ms not attributed**
by these coarse counters. This **does not prove** that the remaining time was
rendering: CPU stages can overlap, GPU execution is asynchronous, and asset
stalls can be hidden within other work. Use **Visual Profiler** to inspect CPU
and GPU render areas where supported; inspect **Monitors** for draw calls,
GPU memory estimates, active physics bodies, and resource counts.

The tool never implies that a Vulkan capability probe guarantees successful
renderer startup. OpenGL GPU timestamp availability depends on backend/driver.

### Export and comparison

Debugger > Monitors > **Save CSV** already exports monitor history and profiler
functions. NoeProfiler extends its profiler section with per-frame fields:
`frame_number`, `frame_time_s`, `process_time_s`,
`physics_time_s`, `physics_frame_time_s`, `script_time_s`,
`spike_over_threshold`, `sample_type`. The CSV retains existing
category columns. Aggregate total profiles are marked `aggregate` and
excluded by the comparison script.

```sh
python3 tests/noe_profiler/compare.py baseline.csv candidate.csv --max-p95-regression-pct 10 --json report.json
```

It measures mean/p50/p95/p99 and spikes; a nonzero exit indicates a p95
regression or insufficient data. Configure low-end test machines with the same
project, window, driver, scene and representative warmup. Store baseline
captures separately; never compare cold and warmed loads by mistake.

### Startup and editor initialization

Use the engine's existing `--startup-benchmark` /
`--startup-benchmark-file` options for startup measurements. CPU profiler
samples are acquired only when capture is active and kept within the existing
bounded frame history; NoeProfiler does not activate new global instrumentation
during ordinary editing.

### Further high-priority work

Actual thread scheduling traces, import/load stall markers, render pass GPU
timestamps with safe fallbacks, allocation resource lifetimes, and continuous
low-end hardware CI remain additional tasks. This patch does **not** claim to
have implemented these or a new Vulkan renderer. NoeCore tuning should follow
reproducible traces and before/after low-end captures, not auto-overriding
user-selected graphics settings.
