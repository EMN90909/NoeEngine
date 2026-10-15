# NoeShader v0.1 — shader pack (experimental)

NoeShader is the user-facing name for this **optional shader/material pack**. Existing Godot-derived renderer backends, native shader class names, shader compiler, and import paths remain unchanged for compatibility. This is not a rename of every engine shader or a replacement for built-in rendering.

## Existing shader inventory (confirmed representative files)

| Engine file | Current function |
| --- | --- |
| `servers/rendering/renderer_rd/shaders/forward_clustered/scene_forward_clustered.glsl` | Vulkan/RD clustered forward scene geometry, vertex attributes, lighting and material shader variants |
| `servers/rendering/renderer_rd/shaders/particles.glsl` | GPU compute particle simulation, collisions, attractors, trails and buffers |
| `servers/rendering/renderer_rd/shaders/environment/sky.glsl` | Environment sky pass, camera orientation, multiview and sky sampling |
| `servers/rendering/renderer_rd/shaders/effects/tonemap.glsl` | Tone map/compositing, exposure, glow and color correction bindings |
| `scene/resources/sky.cpp` | Sky resource configuration, radiance size and update mode (not a shader) |

This is **not** a verified complete scan of every shader in the tree. A full inventory requires a recursive repository checkout and renderer compilation.

## Included

- `materials/noe_3dcore.gdshader`: inspectable PBR material with optional albedo, ORM, normal, emission, UV scale and alpha cutoff.
- `vfx/noe_gpu_sparks.gdshader`: particle process shader for sparks.
- `vfx/noe_impact.gdshader`: radial impact burst process shader.
- `vfx/noe_smoke.gdshader`: alpha-blended smoke material for textured billboards and particle meshes.
- `atmosphere/noe_procedural_sky.gdshader`: parameterized sky gradient, sun disc/halo and layered noise clouds.
- `atmosphere/noe_depth_fog.gdshader`: forward-renderer full-screen distance/height fog overlay.
- `post/noe_color_grade.gdshader`: full-screen grade with exposure, lift/gamma/gain, saturation, contrast, local bright-pass bloom approximation, vignette and chromatic aberration.

## Use without editing shader core

1. Create a `ShaderMaterial`, load one of the `.gdshader` files and adjust inspector uniforms. These are public artist controls; editing shader source is optional.
2. For 3DCore, assign to a `MeshInstance3D`. Its maps are optional; leave toggles off for a solid-color material.
3. For sparks/impact, set `GPUParticles3D.process_material` to a ShaderMaterial using the particle shader and provide an appropriate draw-pass mesh/material. For one-shot impacts, configure the emitter's `one_shot` property.
4. For smoke, use a textured mesh or particle draw-pass material with UVs and transparent sorting.
5. For the sky, create `Sky` + `ShaderMaterial` with `noe_procedural_sky.gdshader`, then select it in `WorldEnvironment`.
6. For post effects, attach `noe_color_grade.gdshader` to a full-window `ColorRect` (in a `CanvasLayer`); confirm screen capture behavior in the chosen renderer. The included bloom is a compact 5x5 blur, **not** a multi-resolution HDR bloom renderer.
7. For fog, use a full-screen QuadMesh with the shader and camera-aligned placement; the shader assumes a 0..1 depth-range rendering backend. Do not enable it on Compatibility without adapting the depth reconstruction.

## Performance and compatibility

The post shader uses 25 screen texture taps per pixel plus chromatic aberration. Use it selectively on lower-end GPUs. The procedural clouds use noise samples; disabling them reduces GPU work. Built-in volumetric fog and environment glow may outperform the sample overlay in fidelity or performance.

## Validation before merging

- Import all seven shader resources in a NoeEngine v0.1 editor build; check shader compile errors per backend.
- Preview PBR/normal/ORM maps, transparency and 3D lighting.
- Exercise smoke, spark and impact GPUParticles3D scenes, including one-shot and paused/time-scale cases.
- Compare sky at sunrise/noon/night; check cloud motion.
- Confirm depth reconstruction and UI layering, and profile 1080p/4K.
- Run a full SCons editor build and shader compilation tests on target platforms.

No engine build or runtime shader compilation has been performed by this PR authoring environment.