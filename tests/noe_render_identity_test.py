#!/usr/bin/env python3
"""Regression checks for user-facing NoeRender identity and legacy project IDs.

Run from the repository root: python3 tests/noe_render_identity_test.py
These are source-level assertions, NOT renderer runtime or GPU validation.
"""
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]
PROJECT_MANAGER = ROOT / "editor" / "project_manager.cpp"

class NoeRenderIdentityTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.source = PROJECT_MANAGER.read_text(encoding="utf-8")

    def test_display_brand(self):
        self.assertIn('rs_button->set_text(TTR("NoeRender (Forward+)"));', self.source)

    def test_serialized_ids_remain_supported(self):
        for renderer_id in ("forward_plus", "mobile", "gl_compatibility"):
            with self.subTest(renderer_id=renderer_id):
                self.assertIn(f'set_meta(SNAME("rendering_method"), "{renderer_id}")', self.source)

    def test_forward_plus_project_setting_unchanged(self):
        self.assertIn('initial_settings["rendering/renderer/rendering_method"] = renderer_type;', self.source)
        self.assertIn('if (renderer_type == "forward_plus")', self.source)

if __name__ == "__main__":
    unittest.main()
