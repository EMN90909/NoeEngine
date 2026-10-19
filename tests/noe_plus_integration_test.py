#!/usr/bin/env python3
"""Noe+ source-level integration regressions (NOT a graphics smoke test)."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[1]
class NoePlusTests(unittest.TestCase):
    def test_quality_initializes_after_renderer_selection(self):
        code=(ROOT/"main/main.cpp").read_text()
        self.assertLess(code.index('rendering_method = GLOBAL_GET("rendering/renderer/rendering_method")'),code.index("NoePlus::configure("))
    def test_preserves_serialized_renderer_ids(self):
        code=(ROOT/"main/main.cpp").read_text()
        for name in ["forward_plus","mobile","gl_compatibility"]:
            self.assertIn(f'rendering_method == "{name}"',code)
    def test_runtime_affects_3d_only(self):
        code=(ROOT/"scene/main/scene_tree.cpp").read_text()
        self.assertIn("root->set_scaling_3d_scale(next_scale)",code)
    def test_common_adapter_does_not_include_windows_platform_headers(self):
        code=(ROOT/"main/noe_render_quality.h").read_text()
        self.assertNotIn('"editor/noe_hardware_profile.h"',code)
        self.assertNotIn("<windows.h>",code)
    def test_editor_isolation(self):
        code=(ROOT/"main/noe_render_quality.h").read_text()
        self.assertIn("if (editor_or_project_manager) return;",code)
if __name__=="__main__": unittest.main()
