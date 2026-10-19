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
                if host == "windows":
                    self.assertIn("mingw_prefix=", args)
                else:
                    self.assertNotIn("mingw_prefix=", args)
                self.assertIn("platform=windows", args)
                self.assertIn("-j2", args)
                self.assertFalse(any("msvc" in value.lower() for value in args))
    def test_preflight_checks_gcc_cpp_and_resource_compiler(self):
        for host in ("windows", "linux"):
            with self.subTest(host=host), patch.object(module, "host_platform", return_value=host), \
                 patch.object(module, "check_tool") as check:
                module.verify_toolchain("windows")
                tools = {call.args[0] for call in check.call_args_list}
                expected = ({"gcc", "g++", "windres"} if host == "windows" else
                            {"x86_64-w64-mingw32-gcc", "x86_64-w64-mingw32-g++",
                             "x86_64-w64-mingw32-windres"})
                self.assertTrue(expected.union({"scons", "makensis"}).issubset(tools))
                self.assertNotIn("cl", tools)
    def test_windows_compiler_detection_avoids_shell_and_reports_failure(self):
        detector = (ROOT / "platform/windows/detect.py").read_text()
        probe = detector.split("def try_cmd(", 1)[1].split("def can_build(", 1)[0]
        self.assertIn("shutil.which(executable)", probe)
        self.assertIn("subprocess.run(", probe)
        # A source comment may mention shell=True; reject the actual kwarg.
        self.assertNotIn("shell=True,", probe)
        self.assertIn("result.returncode == 0", probe)
        self.assertIn("file=sys.stderr", probe)
    def test_windres_errors_are_visible_not_swallowed(self):
        resource = (ROOT / "platform/windows/detect.py").read_text()
        body = resource.split("def build_res_file(", 1)[1].split("def setup_msvc_manual(", 1)[0]
        self.assertIn('subprocess.run(command, check=False)', body)
        self.assertIn('file=sys.stderr', body)
        self.assertIn('result.returncode != 0', body)
        self.assertNotIn('stderr=subprocess.PIPE', body)
        ps1 = (ROOT / "misc/setup_windows_build.ps1").read_text()
        self.assertIn("'windres.exe'", ps1)
        self.assertIn('Invoke-VisibleNative', ps1)
        self.assertIn("printf 'y\\n' | pacman", ps1)
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
