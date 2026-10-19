# NoeRender deterministic startup / 3D scene smoke test

This small Godot 4.0-compatible project generates an 81-box 3D scene with a
camera and directional light, measures 180 **main-loop** frame intervals and exits.

With a compiled editor:

```sh
./bin/noeengine.linuxbsd.editor.x86_64 --headless --path tests/noe_render_benchmark
```

Expected stdout includes `NOERENDER_SMOKE:{...}`.

**Not a GPU benchmark:** `--headless` does not render the actual Vulkan/OpenGL
frame output. For meaningful GPU baseline measurements, rerun with a real
display, fixed resolution, warm/cold shader-cache conditions and the same
GPU/driver against identical scene revisions. Record p50/p95/p99 frame times,
CPU/RAM and actual GPU metrics with external profiling where supported.

The project deliberately uses `Custom` quality mode to keep reference
render settings fixed; test each NoeRender tier in a separate controlled session,
never compare unlike resolutions or renderers as if they were equivalent.
