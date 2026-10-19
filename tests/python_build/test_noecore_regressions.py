"""Non-invasive NoeCore integration invariants."""
import pathlib
import unittest

ROOT = pathlib.Path(__file__).resolve().parents[2]

class TestNoeCore(unittest.TestCase):
    def test_advisory_quality(self):
        main = (ROOT / "main/main.cpp").read_text()
        hooks = (ROOT / "main/noe_core_integration.h").read_text()
        self.assertIn("NoeCore::initialize(noe_core_gpu)", main)
        self.assertNotIn('set_setting("rendering/noe_render/quality/preset"', hooks)
        self.assertIn("advisory only", hooks)

    def test_session_privacy(self):
        recovery = (ROOT / "editor/noe_session_recovery.h").read_text()
        self.assertIn('file->store_string("active\\n")', recovery)
        self.assertIn("atomic_save_layout", recovery)
        self.assertNotIn("API_KEY", recovery)
        self.assertIn("DirAccess::rename_absolute", recovery)
        self.assertIn("NoeSessionRecovery::finish()", (ROOT / "editor/editor_node.cpp").read_text())

    def test_editor_cache_non_destructive(self):
        previews = (ROOT / "editor/editor_resource_preview.cpp").read_text()
        self.assertIn("get_reference_count()", previews)
        self.assertIn("interface/noecore/preview_cache_budget_mb", previews)
        self.assertIn('ResourceImporterTexture::get_singleton()->update_imports();', (ROOT / "editor/editor_node.cpp").read_text())

if __name__ == "__main__":
    unittest.main()
