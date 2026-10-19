# Matched NoeRender graphics benchmark methodology

The project in this folder must be run with a **real graphics display**.
The recorded `frame_ms_p50/p95/p99` samples are main-loop timings; optional
`--gpu-profile` gives additional backend logs when supported. Never call
software Mesa/Xvfb a physical GPU test.

Run matching before/after source builds, at fixed 1280x720, on the same GPU,
OS, driver, renderer, preset, thermal/power state, screen refresh, and scene
source hash. On Linux, record `glxinfo -B` and `vulkaninfo --summary` in
the test report. Explicitly tag the physical device and driver:

```bash
export NOERENDER_DEVICE_TAG="Intel HD Graphics 2500 (physical)"
export NOERENDER_DRIVER_TAG="Mesa 25.x.x / Linux x.y"
python3 tests/noe_render_benchmark/runner.py --engine ./bin/noeengine.linuxbsd.editor.x86_64 --renderer gl_compatibility --output /tmp/noerender-before
# Rebuild the changed source, while preserving the SAME scene hash and settings:
python3 tests/noe_render_benchmark/runner.py --engine ./bin/noeengine.linuxbsd.editor.x86_64 --renderer gl_compatibility --output /tmp/noerender-after
python3 tests/noe_render_benchmark/compare_metrics.py --before /tmp/noerender-before/metrics.json --after /tmp/noerender-after/metrics.json --report /tmp/noerender-performance-diff.json
python3 tests/noe_render_benchmark/compare.py --reference /tmp/noerender-before/render.png --actual /tmp/noerender-after/render.png --threshold 5 --report /tmp/noerender-visual-diff.json
```

Do repeated trials, separate warm/cold shader-cache runs, and archive compiler
SHA, complete `engine.log`, renderer screenshot, ambient/GPU thermals and
device diagnostics. Positive/negative deltas are descriptive, not proof of
a GPU speedup or significance. The comparator rejects missing or mismatched
hardware labels and software-renderer-only samples.
