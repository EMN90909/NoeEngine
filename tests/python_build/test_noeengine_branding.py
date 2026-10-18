"""Fast regressions for NoeEngine branding and the editor theme."""
from pathlib import Path
import subprocess
import sys
import unittest

ROOT = Path(__file__).resolve().parents[2]


class TestNoeEngineBranding(unittest.TestCase):
    def test_branding_sources_are_already_applied(self):
        result = subprocess.run(
            [sys.executable, str(ROOT / "misc/apply_noeengine_branding.py"), "--check"],
            cwd=ROOT, text=True, capture_output=True,
        )
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)

    def test_native_executable_and_config_identity(self):
        version = (ROOT / "version.py").read_text(encoding="utf-8")
        self.assertIn('name = "NoeEngine"', version)
        self.assertIn('short_name = "noeengine"', version)
        for platform in ("linuxbsd", "windows", "macos"):
            scons = (ROOT / "platform" / platform / "SCsub").read_text(encoding="utf-8")
            self.assertIn("#bin/noeengine", scons)
        packaging = (ROOT / "misc/build_products.py").read_text(encoding="utf-8")
        self.assertIn('"noeengine.linuxbsd.editor.x86_64*"', packaging)
        self.assertIn('"noeengine.windows.editor.x86_64.exe"', packaging)
        self.assertIn('"noeengine.macos.editor.*"', packaging)

    def test_black_default_theme(self):
        settings = (ROOT / "editor/editor_settings.cpp").read_text(encoding="utf-8")
        theme = (ROOT / "editor/editor_themes.cpp").read_text(encoding="utf-8")
        self.assertIn('"interface/theme/preset", "NoeEngine Dark"', settings)
        self.assertIn('Color(0.075, 0.08, 0.09)', settings)
        self.assertIn('Color(0.72, 0.75, 0.80)', settings)
        self.assertIn('NoeEngine Dark and legacy Default presets', theme)
        self.assertIn('preset_draw_extra_borders = true;', theme)

    def test_source_attribution_and_godot_compatibility_remain(self):
        self.assertIn('based on Godot Engine (MIT)',
                      (ROOT / "editor/editor_about.cpp").read_text(encoding="utf-8"))
        self.assertIn('project.godot',
                      (ROOT / "editor/project_manager.cpp").read_text(encoding="utf-8"))
        self.assertIn('modules/gdscript',
                      (ROOT / "modules/noqeri/noqeri_script.h").read_text(encoding="utf-8"))

    def test_build_runs_branding(self):
        build = (ROOT / "build.sh").read_text(encoding="utf-8")
        self.assertIn('python3 misc/apply_noeengine_branding.py', build)


if __name__ == "__main__":
    unittest.main()
