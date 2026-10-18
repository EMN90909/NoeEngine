"""Source-contract tests for signed updater integration. Build/UI tests remain mandatory."""
import pathlib
import unittest
ROOT=pathlib.Path(__file__).resolve().parents[2]
CPP=(ROOT/'editor/noe_update_manager.cpp').read_text()
HEADER=(ROOT/'editor/noe_update_manager.h').read_text()
GO=(ROOT/'tools/noe-updater/main.go').read_text()
class NativeSignedUpdateContracts(unittest.TestCase):
    def test_periodic_fetch_and_user_consent(self):
        self.assertIn('auto_timer->set_wait_time(300.0)',CPP)
        self.assertIn('download_url',CPP)
        self.assertIn('install_dialog->set_ok_button_text',CPP)
        self.assertIn('popup_centered',CPP)
    def test_installer_is_local_and_signatures_required(self):
        self.assertIn('NOEENGINE_PATCH_HELPER',CPP)
        self.assertIn('NOEENGINE_UPDATE_PUBKEY',CPP)
        self.assertIn('create_process(helper, args)',CPP)
        self.assertIn('ed25519.Verify',GO)
        self.assertIn('safeTarget(root,c.Path)',GO)
        self.assertIn('file checksum mismatch',GO)
        self.assertIn('release version mismatch',GO)
    def test_restart_receipt_and_version_guard(self):
        self.assertIn('last_update.json',CPP)
        self.assertIn('NOE_PRODUCT_VERSION',CPP)
        self.assertIn('installed_version',CPP)
        self.assertIn('get_tree()->quit()',CPP)
        self.assertIn('get_process_id()',CPP)
    def test_no_forged_production_domain(self):
        self.assertNotIn('website_url = "https://noeengine-web.onrender.com"',CPP)
        self.assertIn('if (quiet_check) return;',CPP)
    def test_logo_is_real_png(self):
        self.assertTrue((ROOT/'logo.png').read_bytes().startswith(b'\\x89PNG\\r\\n\\x1a\\n'))
if __name__=='__main__':
    unittest.main()
