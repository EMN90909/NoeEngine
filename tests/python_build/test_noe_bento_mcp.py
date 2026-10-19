"""NoeEngine bento UI and privacy-preserving desktop inventory contracts."""
from pathlib import Path
import unittest
ROOT = Path(__file__).resolve().parents[2]

class NativeBentoContracts(unittest.TestCase):
    def test_native_bento_is_memory_gated(self):
        manager=(ROOT / "editor/project_manager.cpp").read_text()
        themes=(ROOT / "editor/editor_themes.cpp").read_text()
        account=(ROOT / "editor/noe_account_workspace.cpp").read_text()
        self.assertIn("const bool noe_bento_layout = !NoeHardwareProfile::use_classic_ui();",manager)
        self.assertIn('NoeBentoPanel',manager)
        self.assertIn('theme->set_stylebox("NoeBentoPanel", "EditorStyles"',themes)
        self.assertIn("const bool bento_account = !NoeHardwareProfile::use_classic_ui();",account)
        self.assertIn('page_cards[i]->set_visible(i == idx)',account)
        self.assertIn('SNAME("NoeViper")',account)
    def test_mcp_inventory_sends_only_names_and_flags(self):
        editor=(ROOT / "editor/noe_account_workspace.cpp").read_text()
        method=editor.split("void NoeAccountWorkspace::_sync_mcp_inventory() {",1)[1].split("void NoeAccountWorkspace::_render_integrations()",1)[0]
        self.assertIn('payload["device_id"] = bridge_endpoint_id;',method)
        self.assertIn('item["name"] = name;',method)
        self.assertIn('item["enabled"]',method)
        self.assertNotIn('item["url"]',method)
        self.assertNotIn('item["key"]',method)
        self.assertIn('Authorization: Bearer',method)
        self.assertIn('auth_base + "/api/engine/mcp-sync"',method)

if __name__ == "__main__":
    unittest.main()
