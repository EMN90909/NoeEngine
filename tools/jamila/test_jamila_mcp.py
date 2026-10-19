#!/usr/bin/env python3
# Copyright (c) 2026 Noethric.
# SPDX-License-Identifier: LicenseRef-Noethric-Proprietary
"""Offline integration tests: actual stdio MCP, approval, playtest, and export.

Uses a tiny executable stub instead of assuming GitHub runners can compile the
entire Godot fork. Successful stub tests do not prove a real engine build.
"""
import json
import os
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parent))
from noe_mcp_server import ProjectTools, TOOLS
from jamila_agent import JamilaService, normalize_plan, call_model


FAKE_ENGINE = r"""#!/usr/bin/env python3
import pathlib
import sys
args = sys.argv[1:]
if "--export-release" in args or "--export-pack" in args:
    target = pathlib.Path(args[-1])
    target.parent.mkdir(parents=True, exist_ok=True)
    target.write_bytes(b"fake binary" * 32)
print("FAKE ENGINE TEST ONLY")
"""


class JamilaIntegrationTests(unittest.TestCase):
    def setUp(self):
        self.directory = tempfile.TemporaryDirectory()
        self.addCleanup(self.directory.cleanup)
        self.root = Path(self.directory.name) / "game"
        self.root.mkdir()
        (self.root / "project.godot").write_text('[application]\nconfig/name="Test World"\n', encoding="utf8")
        self.engine = Path(self.directory.name) / "fake-noeengine"
        self.engine.write_text(FAKE_ENGINE, encoding="utf8")
        self.engine.chmod(0o755)
        self.scene = self.root / "scenes" / "main.tscn"
        self.scene.parent.mkdir()
        self.scene.write_text('[gd_scene format=3]\n[node name="Root" type="Node2D"]\n', encoding="utf8")
        (self.root / "export_presets.cfg").write_text('[preset.0]\nname="Linux/X11"\n', encoding="utf8")
        self.tools = ProjectTools(self.root, self.engine, "test-approval-token")

    def test_stdio_mcp_initialize_and_tools(self):
        command = [sys.executable, str(Path(__file__).with_name("noe_mcp_server.py")),
                   "--project", str(self.root), "--engine", str(self.engine)]
        requests = [
            {"jsonrpc":"2.0","id":1,"method":"initialize","params":{}},
            {"jsonrpc":"2.0","id":2,"method":"tools/list","params":{}},
            {"jsonrpc":"2.0","id":3,"method":"tools/call",
             "params":{"name":"noe_project_info","arguments":{}}},
        ]
        proc = subprocess.run(command, input="\n".join(map(json.dumps, requests))+"\n",
                              text=True, capture_output=True, timeout=12)
        self.assertEqual(proc.returncode, 0, proc.stderr)
        messages = [json.loads(line) for line in proc.stdout.splitlines()]
        self.assertEqual(len(messages), 3)
        self.assertEqual(messages[0]["result"]["serverInfo"]["name"], "jamila-noeengine")
        self.assertIn("noe_run_scene", [t["name"] for t in messages[1]["result"]["tools"]])
        self.assertIn("noe_export_release", [t["name"] for t in messages[1]["result"]["tools"]])
        info = json.loads(messages[2]["result"]["content"][0]["text"])
        self.assertEqual(info["project_name"], "Test World")

    def test_rejects_traversal_symlinks_and_unapproved_writes(self):
        with self.assertRaises(ValueError):
            self.tools.path("../outside.gd", writing=True)
        with self.assertRaises(ValueError):
            self.tools.path(".env", writing=True)
        outside = Path(self.directory.name) / "outside"
        outside.mkdir()
        (self.root / "linked").symlink_to(outside, target_is_directory=True)
        with self.assertRaises(ValueError):
            self.tools.path("linked/evil.gd", writing=True)
        with self.assertRaises(PermissionError):
            self.tools.call("noe_write_asset", {"path":"test.gd","content":"extends Node"})
        self.assertFalse((self.root / "test.gd").exists())

    def test_scene_playtest_and_release_are_actual_subprocess_calls(self):
        auth = {"approval_token": "test-approval-token"}
        run = self.tools.call("noe_run_scene", {"path":"scenes/main.tscn", **auth})
        self.assertTrue(run["ok"], run)
        release = self.tools.call("noe_export_release", {"preset":"Linux/X11", **auth})
        self.assertTrue(release["ok"], release)
        self.assertEqual(release["path"], "builds/Jamila-Game.x86_64")
        self.assertTrue((self.root / release["path"]).is_file())
        with self.assertRaises(ValueError):
            self.tools.call("noe_export_release", {"preset":"Unconfigured", **auth})

    def test_main_scene_setup_requires_approval_and_preserves_settings(self):
        with self.assertRaises(PermissionError):
            self.tools.call("noe_set_main_scene", {"path":"scenes/main.tscn"})
        result = self.tools.call("noe_set_main_scene", {
            "path":"scenes/main.tscn", "approval_token":"test-approval-token"
        })
        self.assertEqual(result["main_scene"], "res://scenes/main.tscn")
        self.assertIn('run/main_scene="res://scenes/main.tscn"',
                      (self.root/"project.godot").read_text())
        with self.assertRaises(ValueError):
            self.tools.call("noe_set_main_scene", {
                "path":"scenes/main.tscn", "approval_token":"test-approval-token"
            })
        self.assertTrue((self.root/"project.godot.jamila-backup").exists())

    def test_generate_approve_validate_playtest_then_release(self):
        service = JamilaService(self.root, self.engine)
        plan = {"summary":"Create a script", "actions":[
            {"tool":"noe_write_asset","arguments":{
                "path":"scripts/player.gd",
                "content":"extends Node2D\nfunc _ready():\n    pass\n",
                "overwrite":False
            }},
            {"tool":"noe_set_main_scene","arguments":{
                "path":"scenes/main.tscn", "overwrite":False
            }}
        ]}
        with patch("jamila_agent.call_model", return_value=plan):
            proposal = service.plan({"prompt":"Create a player script"})
        self.assertTrue(proposal["plan_id"])
        self.assertFalse((self.root / "scripts/player.gd").exists())
        result = service.apply({"plan_id": proposal["plan_id"]})
        self.assertTrue(result["ok"], result)
        self.assertTrue((self.root / "scripts/player.gd").is_file())
        self.assertIn('run/main_scene="res://scenes/main.tscn"', (self.root/"project.godot").read_text())
        build = service.export_game({"preset":"Linux/X11","mode":"release"})
        self.assertTrue(build["ok"], build)
        self.assertTrue((self.root / "builds/Jamila-Game.x86_64").is_file())
        with self.assertRaises(ValueError):
            service.apply({"plan_id":proposal["plan_id"]})

    def test_review_plan_cannot_overwrite_subsequent_user_edits(self):
        service = JamilaService(self.root, self.engine)
        plan = {"summary":"Adjust scene", "actions":[
            {"tool":"noe_write_asset","arguments":{
                "path":"scenes/main.tscn", "content":'[gd_scene format=3]\n[node name="Changed" type="Node2D"]\n',
                "overwrite":True
            }}
        ]}
        with patch("jamila_agent.call_model", return_value=plan):
            proposal = service.plan({"prompt":"Improve my scene"})
        self.scene.write_text("user edited this after planning\n", encoding="utf8")
        with self.assertRaisesRegex(ValueError, "changed since planning"):
            service.apply({"plan_id":proposal["plan_id"]})
        self.assertEqual(self.scene.read_text(), "user edited this after planning\n")

    def test_cloud_uses_signed_in_desktop_token_not_provider_keys(self):
        class FakeResponse:
            def __enter__(self): return self
            def __exit__(self, *args): return False
            def read(self, *args):
                plan={'summary':'safe','actions':[{'tool':'noe_write_asset','arguments':{
                    'path':'player.gd','content':'extends Node','overwrite':False}}]}
                return json.dumps({'choices':[{'message':{'content':json.dumps(plan)}}]}).encode()
        observed=[]
        def fake_open(req, timeout):
            observed.append(req)
            return FakeResponse()
        with patch('jamila_agent.urllib.request.urlopen', fake_open):
            out=call_model('https://noeengine-web.onrender.com/api/jamila/plan',
                           'desktop.'+'x'*56,'google','gameplay','make a player',{})
        self.assertEqual(out['actions'][0]['arguments']['path'], 'player.gd')
        data=json.loads(observed[0].data)
        self.assertEqual(data['provider'],'google')
        self.assertEqual(data['role'],'gameplay')
        self.assertNotIn('api_key',data)
        self.assertIn('Bearer desktop.',observed[0].get_header('Authorization'))
    def test_plan_rejects_unapproved_tools(self):
        with self.assertRaises(ValueError):
            normalize_plan({"summary":"unsafe", "actions":[{"tool":"shell", "arguments":{}}]})
        self.assertTrue(any(tool["name"] == "noe_validate_project" for tool in TOOLS))


if __name__ == "__main__":
    unittest.main()
