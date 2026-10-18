"""Native collaboration source contracts. Does not replace a compiled two-user test."""
import pathlib
import unittest
ROOT=pathlib.Path(__file__).resolve().parents[2]
SOURCE=(ROOT/"editor/noe_account_workspace.cpp").read_text()
HEADER=(ROOT/"editor/noe_account_workspace.h").read_text()
class CollaborationContracts(unittest.TestCase):
    def test_real_editor_saves_are_wired(self):
        self.assertIn('connect("scene_saved"',SOURCE)
        self.assertIn('connect("resource_saved"',SOURCE)
        self.assertIn('void NoeAccountWorkspace::_send_collaboration_event',SOURCE)
        self.assertIn('payload["action"] = "saved"',SOURCE)
    def test_secure_device_identity_and_minimal_event(self):
        self.assertIn('Authorization: Bearer ',SOURCE)
        self.assertIn('if (!online_session || access_token.is_empty()',SOURCE)
        self.assertIn('payload["path"] = relative_path;',SOURCE)
        self.assertNotIn('payload["actorId"]',SOURCE)
        self.assertNotIn('payload["fileContents"]',SOURCE)
    def test_activity_feed_is_member_scoped_on_backend(self):
        self.assertIn('/v1/collaboration/activity?projectId=',SOURCE)
        self.assertIn('collab_timer->set_wait_time(15.0)',SOURCE)
        self.assertIn('No teammate changes yet.',SOURCE)
        self.assertIn('LineEdit *collab_project',HEADER)
    def test_no_baked_in_backend_urls(self):
        self.assertIn('NOEENGINE_COLLAB_URL',SOURCE)
        self.assertNotIn('collaboration_url = "https://',SOURCE)
if __name__=="__main__": unittest.main()
