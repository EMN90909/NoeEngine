"""NoeEngine Noqeri editor addon contract and non-destructive installer tests."""
import importlib.util
from pathlib import Path
import tempfile
import unittest
from unittest import mock

ROOT = Path(__file__).resolve().parents[2]
INTEGRATION = ROOT / "misc" / "noeengine_noqeri"
spec = importlib.util.spec_from_file_location("noe_noqeri_installer", INTEGRATION / "install.py")
installer = importlib.util.module_from_spec(spec)
spec.loader.exec_module(installer)


class NoqeriIntegrationTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.base = Path(self.tmp.name)
        self.project = self.base / "game"
        self.project.mkdir()
        self.project_file = self.project / "project.godot"
        self.project_file.write_text("config_version=5\n")

    def test_addon_installs_without_changing_project_settings(self):
        before = self.project_file.read_bytes()
        out = installer.install(self.project)
        self.assertTrue((out / "plugin.cfg").is_file())
        self.assertTrue((out / "plugin.gd").is_file())
        self.assertTrue((out / "noqeri_importer.gd").is_file())
        self.assertTrue((out / "examples/hello.nqr").is_file())
        self.assertEqual(before, self.project_file.read_bytes())

    def test_existing_addon_is_not_overwritten(self):
        original = self.project / "addons/noeengine_noqeri"
        original.mkdir(parents=True)
        existing = original / "important.txt"
        existing.write_text("do not lose")
        with self.assertRaises(FileExistsError):
            installer.install(self.project)
        self.assertEqual(existing.read_text(), "do not lose")

    def test_install_requires_valid_project(self):
        self.project_file.unlink()
        with self.assertRaises(ValueError):
            installer.install(self.project)

    def test_required_resource_and_editor_contracts(self):
        plugin = (INTEGRATION / "addons/noeengine_noqeri/plugin.gd").read_text()
        importer = (INTEGRATION / "addons/noeengine_noqeri/noqeri_importer.gd").read_text()
        self.assertIn("OS.execute(compiler_path, arguments, messages, true)", plugin)
        self.assertIn('PackedStringArray([command, ProjectSettings.globalize_path(current_path)])', plugin)
        self.assertIn('return PackedStringArray(["nqr"])', importer)
        self.assertIn("add_import_plugin(importer)", plugin)
        self.assertIn("remove_import_plugin(importer)", plugin)
        self.assertIn('begins_with("res://")', plugin)
        self.assertIn("run_confirmation.confirmed.connect(_run_legacy)", plugin)


if __name__ == "__main__":
    unittest.main()
