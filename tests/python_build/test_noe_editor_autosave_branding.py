"""Static guardrails for NoeEngine identity and safe periodic auto-save.

These tests inspect C++ source invariants; they do not replace compilation,
headless editor tests or GUI smoke checks on supported operating systems.
"""
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[2]

def read(path):
    return (ROOT / path).read_text(encoding="utf8")

class NoeEditorBehaviorTests(unittest.TestCase):
    def test_noerender_is_user_facing_only(self):
        node = read("editor/editor_node.cpp")
        manager = read("editor/project_manager.cpp")
        self.assertIn('renderer->add_item(TTR("NoeRender"))', node)
        self.assertIn('rs_button->set_text(TTR("NoeRender"))', manager)
        self.assertNotIn('renderer->add_item(TTR("Forward+"))', node)
        self.assertNotIn('rs_button->set_text(TTR("Forward+"))', manager)
        self.assertIn('rendering_method == "forward_plus"', node)
        self.assertIn('SNAME("rendering_method"), "forward_plus"', manager)

    def test_autosave_is_configurable_and_skips_unsaved_or_running_scenes(self):
        settings = read("editor/editor_settings.cpp")
        node = read("editor/editor_node.cpp")
        self.assertIn('"run/auto_save/save_modified_scenes_periodically", true', settings)
        self.assertIn('"run/auto_save/interval_seconds", 120, "30,1800,10"', settings)
        self.assertIn('"text_editor/behavior/files/autosave_interval_secs", 120', settings)
        self.assertIn('void EditorNode::_noe_autosave_tick()', node)
        self.assertIn('editor_run.get_status() != EditorRun::STATUS_STOP', node)
        self.assertIn('filename.is_empty()', node)
        self.assertIn('is_history_unsaved(history_id)', node)
        self.assertNotIn('if (!editor_data.is_scene_changed(i))', node)
        self.assertIn('noe_autosave_timer->set_wait_time(5.0)', node)

    def test_internal_compatibility_and_upstream_credits_stay_intact(self):
        cli = read("main/main.cpp")
        errors = read("core/error/error_macros.cpp")
        self.assertIn('"user://logs/noeengine.log"', cli)
        self.assertIn('ERROR: [NoeEngine]', errors)
        self.assertIn('Godot Engine contributors', cli)
        self.assertIn('project.godot', cli)

if __name__ == '__main__':
    unittest.main()
