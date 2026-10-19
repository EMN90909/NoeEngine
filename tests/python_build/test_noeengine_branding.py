"""Fast regressions for NoeEngine branding and the editor theme."""
from pathlib import Path
import subprocess
import shutil
import tempfile
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

    def test_soft_default_and_classic_lightweight_theme(self):
        settings = (ROOT / "editor/editor_settings.cpp").read_text(encoding="utf-8")
        theme = (ROOT / "editor/editor_themes.cpp").read_text(encoding="utf-8")
        self.assertIn('"interface/theme/preset", noe_default_theme', settings)
        self.assertIn('NoeHardwareProfile::use_classic_ui()', settings)
        profile = (ROOT / "editor/noe_hardware_profile.h").read_text(encoding="utf-8")
        self.assertIn('GlobalMemoryStatusEx', profile)
        self.assertIn('hw.memsize', profile)
        self.assertIn('/proc/meminfo', profile)
        self.assertIn('"NoeEngine Soft,NoeEngine Dark,Default', settings)
        self.assertIn('preset == "NoeEngine Soft"', theme)
        self.assertIn('Color(0.075, 0.08, 0.09)', settings)
        self.assertIn('Color(0.72, 0.75, 0.80)', settings)
        self.assertIn('NoeEngine Dark and legacy Default presets', theme)
        self.assertIn('preset_draw_extra_borders = true;', theme)

    def test_ram_profile_threshold_with_native_compiler(self):
        compiler = shutil.which("c++") or shutil.which("g++")
        if not compiler:
            self.skipTest("Native C++ compiler unavailable")
        source = """#include "editor/noe_hardware_profile.h"
int main() {
  constexpr unsigned long long gib = 1024ULL * 1024 * 1024;
  if (!NoeHardwareProfile::use_classic_ui(0)) return 1;
  if (!NoeHardwareProfile::use_classic_ui(3 * gib)) return 2;
  if (!NoeHardwareProfile::use_classic_ui(4 * gib)) return 3;
  if (NoeHardwareProfile::use_classic_ui(4 * gib + 1)) return 4;
  if (NoeHardwareProfile::use_classic_ui(6 * gib)) return 5;
  return 0;
}
"""
        with tempfile.TemporaryDirectory() as directory:
            cpp = Path(directory) / "profile.cpp"
            executable = Path(directory) / "profile"
            cpp.write_text(source, encoding="utf-8")
            built = subprocess.run(
                [compiler, "-std=c++17", "-DLINUXBSD_ENABLED", "-I", str(ROOT),
                 str(cpp), "-o", str(executable)],
                cwd=ROOT, text=True, capture_output=True, timeout=45,
            )
            self.assertEqual(built.returncode, 0, built.stdout + built.stderr)
            tested = subprocess.run([str(executable)], cwd=ROOT, timeout=10)
            self.assertEqual(tested.returncode, 0)

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
