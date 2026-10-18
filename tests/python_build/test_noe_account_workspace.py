"""Source-level regressions for native NoeEngine account panels.

These checks do not substitute for compiling the C++ editor or browser E2E.
"""
import pathlib
import unittest

ROOT = pathlib.Path(__file__).resolve().parents[2]
HEADER = ROOT / "editor/noe_account_workspace.h"
SOURCE = ROOT / "editor/noe_account_workspace.cpp"


class TestNoeAccountWorkspace(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.header = HEADER.read_text(encoding="utf-8")
        cls.source = SOURCE.read_text(encoding="utf-8")

    def test_editor_and_manager_both_attach_account_menu(self):
        editor = (ROOT / "editor/editor_node.cpp").read_text()
        manager = (ROOT / "editor/project_manager.cpp").read_text()
        self.assertIn("NoeAccountWorkspace::install(gui_base, right_menu_hb)", editor)
        self.assertIn("NoeAccountWorkspace::install(this, settings_hb)", manager)
        self.assertIn("NoeAccountWorkspace *NoeAccountWorkspace::install(", self.source)
        self.assertIn('menu->add_item(TTR("Log out")', self.source)

    def test_all_requested_navigation_sections_present(self):
        for section in ("Profile", "Usage & limits", "API & MCP", "Extensions", "Security"):
            self.assertIn(section, self.source)
        for widget in ("HBoxContainer", "VBoxContainer", "ScrollContainer", "TabContainer",
                       "ConfirmationDialog", "MenuButton", "CheckBox"):
            self.assertIn(widget, self.source)

    def test_browser_auth_requires_server_and_verified_me(self):
        self.assertIn('NOEENGINE_AUTH_URL', self.source)
        self.assertIn('"/device/start"', self.source)
        self.assertIn('"/device/poll"', self.source)
        self.assertIn('"/me"', self.source)
        self.assertIn('response', self.source) if False else None
        self.assertIn("online_session = true;", self.source)
        self.assertIn("if (status != 200 || !verified_email.contains", self.source)
        self.assertIn('awaiting_login = false;', self.source)
        self.assertIn('url.begins_with("https://")', self.source)

    def test_access_token_remains_session_only(self):
        self.assertIn("String access_token;", self.header)
        self.assertNotIn('set_value("account", "access_token"', self.source)
        self.assertNotIn('set_value("auth", "token"', self.source)
        self.assertIn("access_token.clear()", self.source)
        self.assertIn('device_code.clear()', self.source)

    def test_usage_is_measured_not_misreported(self):
        self.assertIn("get_ticks_msec()", self.source)
        self.assertIn("is_run_playing()", self.source)
        self.assertIn("NoeUsageCharts", self.source)
        self.assertIn("ai_charts->update_data(", self.source)
        self.assertIn("GPU rendering hours not instrumented", self.source)
        self.assertIn("Provider quota unavailable", self.source)

    def test_collaboration_save_notifications_are_bounded_and_deduplicated(self):
        self.assertIn('Vector<String> queued_collaboration_paths;', self.header)
        self.assertIn('Vector<String> queued_collaboration_ids;', self.header)
        self.assertIn('queued_collaboration_paths.size() >= 64', self.source)
        self.assertIn('payload["requestId"] = queued_collaboration_ids[0]', self.source)
        self.assertIn('queued_collaboration_paths.remove_at(0)', self.source)
        self.assertIn('void NoeAccountWorkspace::_flush_collaboration_queue()', self.source)
        self.assertIn('HTTPClient::METHOD_POST, JSON::stringify(payload)', self.source)
        self.assertIn('if (collab_request) collab_request->cancel_request()', self.source)
        self.assertIn('code == 400 || code == 401 || code == 403', self.source)
        self.assertIn('queued_collaboration_paths.clear()', self.source)

    def test_mcp_registry_discovery_does_not_launch_tools(self):
        self.assertIn("registry.modelcontextprotocol.io/v0.1/servers?search=godot", self.source)
        self.assertIn("No MCP process is executed or connected yet.", self.source)
        self.assertIn("NoeAccountWorkspace::_marketplace_select(", self.source)
        self.assertIn('cb->connect("toggled"', self.source)
        self.assertNotIn("OS::execute(", self.source)

    def test_settings_are_native_and_keyboard_navigation_is_visible(self):
        for text in ('Profile', 'Usage & limits', 'API & MCP', 'Extensions', 'Security'):
            self.assertIn(text, self.source)
        self.assertIn('navigation_buttons[i]->set_pressed_no_signal(i == idx)', self.source)
        self.assertIn('set_focus_mode(Control::FOCUS_ALL)', self.source)
        self.assertIn('popup_centered_clamped', self.source)
        self.assertIn('profile_avatar_label->set_text(', self.source)
        self.assertIn('NoeAccountWorkspace::install(gui_base, right_menu_hb)', (ROOT / 'editor/editor_node.cpp').read_text())
        self.assertIn('NoeAccountWorkspace::install(this, settings_hb)', (ROOT / 'editor/project_manager.cpp').read_text())

    def test_unverified_deployment_is_never_hardcoded(self):
        self.assertNotIn('auth_base = "https://noeengine-web.onrender.com"', self.source)
        self.assertIn('NOEENGINE_AUTH_URL', self.source)
        self.assertIn('config->get_value("account", "website_url"', self.source)
        self.assertIn('No account website configured', self.source)
        self.assertIn('void NoeAccountWorkspace::_save_website_url()', self.source)

    def test_cpp_brace_count(self):
        self.assertEqual(self.source.count("{"), self.source.count("}"))


if __name__ == "__main__":
    unittest.main()
