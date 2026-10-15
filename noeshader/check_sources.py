#!/usr/bin/env python3
"""NoeShader source-level smoke checks (not a GLSL compiler or visual benchmark)."""
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]
SHADERS = {
    "materials/noe_3dcore.gdshader": "spatial",
    "vfx/noe_gpu_sparks.gdshader": "particles",
    "vfx/noe_impact.gdshader": "particles",
    "vfx/noe_smoke.gdshader": "spatial",
    "atmosphere/noe_procedural_sky.gdshader": "sky",
    "atmosphere/noe_depth_fog.gdshader": "spatial",
    "post/noe_color_grade.gdshader": "canvas_item",
}
NATIVE = (
    "servers/rendering/renderer_rd/shaders/environment/sky.glsl",
    "servers/rendering/renderer_rd/shaders/effects/tonemap.glsl",
    "servers/rendering/renderer_rd/shaders/particles.glsl",
    "servers/rendering/renderer_rd/shaders/forward_clustered/scene_forward_clustered.glsl",
)

def run():
    for relative, kind in SHADERS.items():
        source = (ROOT / "noeshader" / relative).read_text(encoding="utf-8")
        assert re.search(r"^shader_type\s+" + kind + r"\s*;", source, flags=re.M), relative
        assert source.count("{") == source.count("}"), relative
        assert "void " in source, relative
        assert "#[fragment]" not in source and "#VERSION_DEFINES" not in source, relative
        if kind == "particles":
            assert "TRANSFORM[3].xyz += VELOCITY * DELTA" in source, relative
            assert "CUSTOM.y += DELTA" in source, relative
    for relative in NATIVE:
        source = (ROOT / relative).read_text(encoding="utf-8")
        assert "#VERSION_DEFINES" in source, relative
        assert "#CODE" in source or "#[vertex]" in source, relative
    print("NoeShader: 7 source structure checks passed; native shader interfaces still present.")
    print("WARNING: This does not check shader compilation, renderer performance, or visual output.")

if __name__ == "__main__":
    run()
