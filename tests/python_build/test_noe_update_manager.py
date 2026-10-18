"""Static release integration checks. Native C++ compile and real installer E2E are separate."""
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[2]


class TestNoeUpdater(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.cpp = (ROOT / "editor/noe_update_manager.cpp").read_text()
        cls.h = (ROOT / "editor/noe_update_manager.h").read_text()

    def test_both_desktop_entry_points_offer_updates(self):
        for name in ("editor/editor_node.cpp", "editor/project_manager.cpp"):
            source = (ROOT / name).read_text()
            self.assertIn("#include \"editor/noe_update_manager.h\"", source)
            self.assertIn("NoeUpdateManager::install(", source)

    def test_release_is_scoped_by_platform_and_https_origin(self):
        self.assertIn("/updates/latest?platform=", self.cpp)
        self.assertIn("&channel=stable", self.cpp)
        self.assertIn('url.begins_with("https://")', self.cpp)
        self.assertIn('host.ends_with(".supabase.co")', self.cpp)
        self.assertIn('public/noeengine-patches/releases/', self.cpp)
        self.assertIn('NOEENGINE_AUTH_URL', self.cpp)

    def test_download_requires_explicit_action_and_integrity_check(self):
        self.assertIn('release_dialog->connect("confirmed"', self.cpp)
        self.assertIn('set_download_file(saved_path)', self.cpp)
        self.assertIn('get_sha256(saved_path).to_lower() == patch_sha256', self.cpp)
        self.assertIn('file->get_length() == patch_size', self.cpp)
        self.assertIn('Download and verify the patch?', self.cpp)
        self.assertIn('install_dialog->connect("confirmed"', self.cpp)
        self.assertNotIn('OS::execute(', self.cpp)
        self.assertNotIn('chmod ', self.cpp)

    def test_release_notes_and_continue_state(self):
        self.assertIn('TTR("Continue")', self.cpp)
        self.assertIn("What's new:", self.cpp)
        self.assertIn('NoeEngine updated successfully', self.cpp)

    def test_no_secret_key_embedded(self):
        self.assertNotIn('SUPABASE_SERVICE_ROLE_KEY', self.cpp)
        self.assertNotIn('NOE_DEVICE_TOKEN_SECRET', self.cpp)

    def test_balanced_source_brackets(self):
        self.assertEqual(self.cpp.count("{"), self.cpp.count("}"))


if __name__ == "__main__":
    unittest.main()
