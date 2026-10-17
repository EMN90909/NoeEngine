# Copyright (c) 2026 Noethric.
# SPDX-License-Identifier: LicenseRef-Noethric-Proprietary
"""Offline checks for the in-engine Noqeri ScriptLanguage integration.

These are source-structure checks, not a substitute for the C++ unit tests
and the executable Godot scene smoke test.
"""
import pathlib
import json
import unittest

ROOT = pathlib.Path(__file__).resolve().parents[2]
MODULE = ROOT / "modules/noqeri"


class NativeNoqeriContractTests(unittest.TestCase):
    def test_native_module_registration(self):
        register = (MODULE / "register_types.cpp").read_text()
        config = (MODULE / "config.py").read_text()
        self.assertIn("ScriptServer::register_language(script_language_noqeri)", register)
        self.assertIn("ResourceLoader::add_resource_format_loader(noqeri_loader)", register)
        self.assertIn("ResourceSaver::add_resource_format_saver(noqeri_saver)", register)
        self.assertIn('env.module_add_dependencies("noqeri", ["gdscript"])', config)

    def test_language_and_scene_script(self):
        header = (MODULE / "noqeri_script.h").read_text()
        impl = (MODULE / "noqeri_script.cpp").read_text()
        self.assertIn("class NoqeriScript : public GDScript", header)
        self.assertIn("class NoqeriLanguage : public ScriptLanguage", header)
        self.assertIn('get_extension() const override { return "nqr"; }', header)
        self.assertIn('get_name() const override { return "Noqeri"; }', header)
        self.assertIn("NoqeriScript::reload", impl)
        self.assertIn("GDScript::reload(p_keep_state)", impl)
        self.assertIn("GDScript::set_source_code(translated)", impl)
        self.assertIn('p_extensions->push_back("nqr")', header)

    def test_reject_unsupported_semantics(self):
        transpiler = (MODULE / "noqeri_translator.cpp").read_text()
        for construct in ['"import"', '"unsafe"', '"record"', '"extern"']:
            self.assertIn(construct, transpiler)
        self.assertIn("Unsupported Noqeri block", transpiler)
        self.assertIn('line.begins_with("repeat ")', transpiler)
        self.assertIn("range(max(0, int(", transpiler)
        self.assertIn("Unmatched closing", transpiler)
        self.assertIn("Fixed-width Noqeri types", transpiler)

    def test_native_smoke_scene_and_godot_compatibility(self):
        project = ROOT / "tests/noqeri_scene"
        self.assertTrue((project / "project.godot").is_file())
        self.assertTrue((project / "smoke.tscn").is_file())
        self.assertIn('path="res://main.nqr"', (project / "smoke.tscn").read_text())
        self.assertIn("NOQERI_NATIVE_READY", (project / "main.nqr").read_text())
        self.assertTrue((MODULE / "tests/test_noqeri.h").is_file())

    def test_noqeri_070_source_compatibility_is_pinned_and_honest(self):
        metadata = json.loads((MODULE / "compatibility.json").read_text())
        header = (MODULE / "noqeri_compatibility.h").read_text()
        self.assertEqual(metadata["language_version"], "0.7.0")
        self.assertEqual(metadata["release_status"], "development-unreleased")
        self.assertEqual(metadata["abi_version"], 2)
        self.assertEqual(metadata["language_edition"], "2026")
        self.assertEqual(len(metadata["compiler_source_commit"]), 40)
        self.assertFalse(metadata["compiler_embedded"])
        self.assertFalse(metadata["verified_full_runtime"])
        self.assertIn("LANGUAGE_VERSION = \\"0.7.0\\"", header)
        self.assertIn("ABI_VERSION = 2", header)
        self.assertIn(metadata["compiler_source_commit"], header)

    def test_no_legacy_duplicate_file_importer(self):
        self.assertFalse((ROOT / "misc/noeengine_noqeri/addons").exists())


if __name__ == "__main__":
    unittest.main()
