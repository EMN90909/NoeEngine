"""NoeEngine low-memory UI and compatibility-name regression tests."""
import pathlib, shutil, subprocess, tempfile, unittest
ROOT = pathlib.Path(__file__).resolve().parents[2]
class TestNoeTheme(unittest.TestCase):
    def test_memory_selection(self):
        cc = shutil.which("c++") or shutil.which("g++")
        if not cc: self.skipTest("C++ compiler unavailable")
        source = """#include "editor/noe_hardware_profile.h"
int main() {
  constexpr unsigned long long G = 1024ULL * 1024 * 1024;
  return (!NoeHardwareProfile::use_classic_ui(0) ||
          !NoeHardwareProfile::use_classic_ui(3 * G) ||
          NoeHardwareProfile::use_classic_ui(4 * G) ||
          NoeHardwareProfile::use_classic_ui(8 * G)) ? 1 : 0;
}"""
        with tempfile.TemporaryDirectory() as temp:
            p = pathlib.Path(temp)
            (p/"test.cpp").write_text(source)
            subprocess.run([cc,"-std=c++17","-DLINUXBSD_ENABLED","-I",str(ROOT),str(p/"test.cpp"),"-o",str(p/"test")],check=True,capture_output=True,text=True)
            subprocess.run([str(p/"test")],check=True,timeout=5)
    def test_display_text_and_compatibility(self):
        manager=(ROOT/"editor/project_manager.cpp").read_text()
        themes=(ROOT/"editor/editor_themes.cpp").read_text()
        settings=(ROOT/"editor/editor_settings.cpp").read_text()
        self.assertIn("project.godot",manager)
        self.assertNotIn("supported Godot version",manager)
        self.assertNotIn("project to Godot %s",manager)
        self.assertIn("NoeEngine Retro",settings)
        self.assertIn('preset == "Godot 2"',themes)  # Existing saved user theme.
        self.assertIn("style_popup->set_corner_radius_all(noe_soft_ui",themes)
if __name__ == "__main__": unittest.main()
