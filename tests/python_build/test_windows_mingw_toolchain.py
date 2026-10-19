"""Native Windows build contract: MinGW-only compiler, visible BAT failures."""
import importlib.util
from pathlib import Path
import unittest
from unittest.mock import patch
ROOT = Path(__file__).resolve().parents[2]
spec = importlib.util.spec_from_file_location("noe_build_products", ROOT / "misc/build_products.py")
module = importlib.util.module_from_spec(spec)
spec.loader.exec_module(module)
class TestWindowsMingwBuild(unittest.TestCase):
    def test_windows_compiler_forced_even_if_msvc_installed(self):
        for host in ("windows", "linux"):
            with self.subTest(host=host), patch.object(module, "host_platform", return_value=host), \
                 patch.object(module, "check_tool"), patch.object(module, "command") as execute:
                module.compile_editor("windows", 2)
                args = execute.call_args.args[0]
                self.assertIn("use_mingw=yes", args)
                self.assertIn("platform=windows", args)
                self.assertIn("-j2", args)
                self.assertFalse(any("msvc" in value.lower() for value in args))
    def test_preflight_checks_gcc_cpp_and_resource_compiler(self):
        with patch.object(module, "host_platform", return_value="windows"), \
             patch.object(module, "check_tool") as check:
            module.verify_toolchain("windows")
            tools = {call.args[0] for call in check.call_args_list}
            self.assertTrue({"x86_64-w64-mingw32-gcc", "x86_64-w64-mingw32-g++",
                             "x86_64-w64-mingw32-windres", "scons", "makensis"}.issubset(tools))
            self.assertNotIn("cl", tools)
    def test_failure_stays_visible_unless_ci_opts_out(self):
        bat = (ROOT / "build.bat").read_text()
        self.assertIn("if /I not", bat)
        self.assertIn("NOEENGINE_NO_PAUSE", bat)
        self.assertIn("pause", bat)
        self.assertIn("BUILD FAILED", bat)
        self.assertIn('set "RESULT=%ERRORLEVEL%"', bat)
        ps1 = (ROOT / "misc/setup_windows_build.ps1").read_text()
        self.assertIn("Start-Transcript", ps1)
        self.assertIn("Stop-Transcript", ps1)
        self.assertIn("MSYS2.MSYS2", ps1)
        self.assertIn("mingw-w64-ucrt-x86_64-gcc", ps1)
        self.assertNotIn("Microsoft.VisualStudio.2022.BuildTools", ps1)
if __name__ == "__main__":
    unittest.main()
