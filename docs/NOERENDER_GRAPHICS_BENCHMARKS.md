# NoeRender graphics benchmark and visual acceptance

These fixtures **render an actual 3D scene**, unlike `--headless`. The scene
contains 361 spheres and nine lights. Warmup is 100 frames, followed by 240
application-frame samples; it records p50/p95/p99, draw calls, primitive counts,
reported memory, a real viewport screenshot and engine logs.

```sh
python3 tests/noe_render_benchmark/runner.py \
 --engine ./bin/noeengine.linuxbsd.editor.x86_64 \
 --renderer gl_compatibility \
 --output Products/noe-render-measured
```

Use physical GPU and a real display. The manual GitHub Actions workflow has a
software Mesa/Xvfb job for CI smoke testing and a separate self-hosted physical
GPU option (`noe-render-gpu` label). Software Mesa results must **never** be
represented as hardware GPU performance. A physical runner must be connected
and available or the hardware job remains queued.

Compare pictures with Pillow, using a *preapproved matching device baseline*:

```sh
python3 tests/noe_render_benchmark/compare.py \
 --reference path/to/approved.png \
 --actual Products/noe-render-measured/render.png \
 --threshold 5 --report Products/noe-render-measured/visual-diff.json
```

Do not silently generate a baseline from the output under test. Verify scene
SHA, GPU, driver, graphics quality, renderer and resolution first. Store
reference screenshots per GPU/driver family with QA approval.

**Limits:** reported frame intervals are **main-loop/application frame times**,
not GPU timestamp queries. Cold/warm shader-cache studies must use controlled
cache copies; profile shader compile stalls with external GPU tools
(RenderDoc, Nsight, Radeon GPU Profiler) before claiming optimization.
Track shader-cache bytes, RAM, VRAM, frame-time p50/p95/p99, visual differences
and trace captures in a release report. No such device-specific results
exist merely because these benchmark scripts were committed.
