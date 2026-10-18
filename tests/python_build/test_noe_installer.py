# Copyright (c) 2026 Noethric.
# SPDX-License-Identifier: LicenseRef-Noethric-Proprietary
"""Installer packaging checks; do not require compiling the full engine."""
import importlib.util
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import unittest
from unittest import mock
from types import SimpleNamespace
import json


SCRIPT = Path(__file__).resolve().parents[2] / "misc/build_products.py"
spec = importlib.util.spec_from_file_location("noe_build_products", SCRIPT)
build = importlib.util.module_from_spec(spec)
spec.loader.exec_module(build)


class InstallerPackagingTests(unittest.TestCase):
    def setUp(self):
        self.directory = tempfile.TemporaryDirectory()
        self.addCleanup(self.directory.cleanup)
        self.root = Path(self.directory.name)
        (self.root / "bin").mkdir()
        (self.root / "Products").mkdir()
        (self.root / "LICENSE.txt").write_text("Upstream MIT test license\n")
        (self.root / "COPYRIGHT.txt").write_text("Upstream copyright test notice\n")
        (self.root / "NOTICE.md").write_text("Godot MIT and Noqeri GPL separate\n")
        (self.root / "NOETHRIC-PROPRIETARY-LICENSE.txt").write_text("Noethric proprietary additions only\n")
        self.patch_root = mock.patch.object(build, "ROOT", self.root)
        self.patch_products = mock.patch.object(build, "PRODUCTS", self.root / "Products")
        self.patch_root.start()
        self.patch_products.start()
        self.addCleanup(self.patch_root.stop)
        self.addCleanup(self.patch_products.stop)

    def test_binary_selection_keeps_upstream_paths(self):
        binary = self.root / "bin/godot.windows.editor.x86_64.exe"
        binary.write_bytes(b"fixture")
        self.assertEqual(build.find_binary("windows"), binary)
        with self.assertRaises(RuntimeError):
            build.find_binary("linux")

    @unittest.skipUnless(sys.platform.startswith("linux"), "Debian packaging runs on Linux")
    @unittest.skipUnless(shutil.which("dpkg-deb"), "dpkg-deb required")
    def test_deb_setup_contains_launcher_and_attribution(self):
        editor = self.root / "bin/godot.linuxbsd.editor.x86_64"
        shutil.copy2(sys.executable, editor)
        result = build.write_debian(editor)
        self.assertEqual(result.name, "NoeEngine-setup.deb")
        self.assertTrue(result.is_file())
        name = subprocess.check_output(["dpkg-deb", "--field", str(result), "Package"], text=True).strip()
        self.assertEqual(name, "noeengine")
        extract = self.root / "unpacked"
        subprocess.run(["dpkg-deb", "--extract", str(result), str(extract)], check=True)
        self.assertTrue((extract / "usr/lib/noeengine/noeengine").is_file())
        self.assertTrue((extract / "usr/share/doc/noeengine/copyright").is_file())
        self.assertTrue((extract / "usr/share/doc/noeengine/NOTICE.md").is_file())
        self.assertIn("noeengine", (extract / "usr/bin/noeengine").read_text())
        self.assertIn("NoeEngine", (extract / "usr/share/applications/noeengine.desktop").read_text())


    def _native_source_fixture(self):
        (self.root / "SConstruct").write_text("# fixture")
        module = self.root / "modules/noqeri"
        module.mkdir(parents=True)
        (module / "noqeri_script.cpp").write_text("// fixture")
        (module / "noqeri_translator.cpp").write_text("// fixture")
        manifest = module / "compatibility.json"
        manifest.write_text(json.dumps({
            "language_version": "0.7.0",
            "abi_version": 2,
            "release_status": "development-unreleased",
        }))
        return manifest

    def test_preflight_detects_expected_native_toolchain(self):
        manifest = self._native_source_fixture()
        with mock.patch.object(build, "NOQERI_COMPAT", manifest), \
                mock.patch.object(build, "host_platform", return_value="linux"), \
                mock.patch.object(build, "check_tool") as check:
            build.verify_toolchain("linux")
            for tool in ("scons", "dpkg-deb", "g++", "pkg-config"):
                check.assert_any_call(tool)

    def test_preflight_fails_closed_on_missing_scons(self):
        manifest = self._native_source_fixture()
        def missing(tool):
            if tool == "scons":
                raise RuntimeError("Required tool not found: scons")
        with mock.patch.object(build, "NOQERI_COMPAT", manifest), \
                mock.patch.object(build, "host_platform", return_value="linux"), \
                mock.patch.object(build, "check_tool", side_effect=missing):
            with self.assertRaisesRegex(RuntimeError, "scons"):
                build.verify_toolchain("linux")

    def test_cli_preflight_does_not_start_compiler(self):
        with mock.patch.object(build, "verify_toolchain") as verify, \
                mock.patch.object(build, "compile_editor") as compile_editor:
            build.main(["linux", "--preflight"])
            verify.assert_called_once_with("linux", package_only=False)
            compile_editor.assert_not_called()

    def test_native_scene_smoke_checks_callbacks_and_repeat(self):
        project = self.root / "tests/noqeri_scene"
        project.mkdir(parents=True)
        (project / "project.godot").write_text("config_version=5")
        editor = self.root / "bin/godot.linuxbsd.editor.x86_64"
        editor.write_bytes(b"fixture executable")
        good = SimpleNamespace(returncode=0,
            stdout="NOQERI_NATIVE_READY\nNOQERI_REPEAT_STEP\nNOQERI_REPEAT_STEP\nNOQERI_NATIVE_PROCESS\n",
            stderr="")
        with mock.patch.object(build, "command"), \
                mock.patch.object(build.subprocess, "run", return_value=good):
            build.smoke_test_linux(editor)
        bad = SimpleNamespace(returncode=0, stdout="NOQERI_NATIVE_READY", stderr="")
        with mock.patch.object(build, "command"), \
                mock.patch.object(build.subprocess, "run", return_value=bad):
            with self.assertRaisesRegex(RuntimeError, "NOQERI_NATIVE_PROCESS"):
                build.smoke_test_linux(editor)

    def test_windows_installer_recipe_has_uninstall_and_credits(self):
        binary = self.root / "bin/godot.windows.editor.x86_64.exe"
        binary.write_bytes(b"MZ fixture")
        captured = []

        def fake_command(arguments, cwd=None):
            script = (Path(cwd) / "installer.nsi").read_text()
            captured.append(script)
            (self.root / "Products/NoeEngine-setup.exe").write_bytes(b"fixture installer")

        with mock.patch.object(build, "check_tool"), mock.patch.object(build, "command", side_effect=fake_command):
            result = build.write_windows(binary)
        self.assertEqual(result.name, "NoeEngine-setup.exe")
        self.assertIn('Name "NoeEngine"', captured[0])
        self.assertIn('File "LICENSE.txt"', captured[0])
        self.assertIn('File "COPYRIGHT.txt"', captured[0])
        self.assertIn('Section "Uninstall"', captured[0])

    def test_macos_pkg_recipe_has_bundle_and_credits(self):
        binary = self.root / "bin/godot.macos.editor.x86_64"
        binary.write_bytes(b"fixture executable")
        captured = []

        def fake_command(arguments, cwd=None):
            app = Path(arguments[2])
            info = (app / "Contents/Info.plist").read_text()
            captured.append(info)
            self.assertTrue((app / "Contents/Resources/LICENSE.txt").exists())
            self.assertTrue((app / "Contents/Resources/COPYRIGHT.txt").exists())
            (self.root / "Products/NoeEngine-setup.pkg").write_bytes(b"fixture pkg")

        with mock.patch.object(build, "check_tool"), mock.patch.object(build, "command", side_effect=fake_command):
            result = build.write_macos(binary)
        self.assertEqual(result.name, "NoeEngine-setup.pkg")
        self.assertIn("xyz.noethric.noeengine", captured[0])
        self.assertIn("NoeEngine", captured[0])

if __name__ == "__main__":
    unittest.main()
