"""Jamila native agent and game MCP regression checks (no AI account needed)."""
import importlib.util
import json
import os
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[2]
JAMILA = ROOT / "tools" / "jamila"
sys.path.insert(0, str(JAMILA))
import jamila_agent as agent
import noe_mcp_server as mcp


class JamilaMcpTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.root = Path(self.tmp.name)
        (self.root / "project.godot").write_text(
            'config_version=5\n[application]\nconfig/name="Jamila Test Game"\n', encoding="utf8")
        self.server = mcp.ProjectTools(self.root, approval="approved-secret")

    def test_discover_project_and_assets(self):
        info = self.server.call("noe_project_info", {})
        self.assertEqual(info["project_name"], "Jamila Test Game")
        self.assertEqual(self.server.call("noe_list_assets", {})["assets"], [])

    def test_write_rejected_without_approval(self):
        args = {"path": "scenes/level.tscn", "content": '[gd_scene format=3]'}
        with self.assertRaises(PermissionError):
            self.server.call("noe_write_asset", args)
        self.assertFalse((self.root / args["path"]).exists())

    def test_approved_scene_write_read_and_summary(self):
        payload = '[gd_scene load_steps=1 format=3]\n[node name="Main" type="Node2D"]\n'
        result = self.server.call("noe_write_asset", {
            "path": "scenes/main.tscn", "content": payload,
            "approval_token": "approved-secret"
        })
        self.assertEqual(result["saved"], "scenes/main.tscn")
        self.assertIn("Main", self.server.call("noe_read_asset", {"path": "scenes/main.tscn"})["content"])
        summary = self.server.call("noe_scene_summary", {"path": "scenes/main.tscn"})
        self.assertEqual(summary["node_count"], 1)
        with self.assertRaises(ValueError):
            self.server.call("noe_write_asset", {
                "path": "scenes/main.tscn", "content": "bad",
                "approval_token": "approved-secret"
            })
        self.assertEqual((self.root / "scenes/main.tscn").read_text(), payload)

    def test_symlink_escape_blocked(self):
        outside = Path(self.tmp.name).parent
        (self.root / "link").symlink_to(outside, target_is_directory=True)
        for path in ["../escaped.gd", "/tmp/escaped.gd", "link/escaped.gd",
                     ".github/workflows/test.gd", ".godot/cache.gd",
                     "res://secret.gd", "scripts\\evil.gd", ".env"]:
            with self.subTest(path=path), self.assertRaises(ValueError):
                self.server.path(path, writing=True)

    def test_validation_only_after_approval_and_real_exit(self):
        with self.assertRaises(PermissionError):
            self.server.call("noe_validate_project", {})
        # No actual engine binary in unit test. Validation must not report success.
        with self.assertRaises(ValueError):
            self.server.call("noe_validate_project", {"approval_token": "approved-secret"})

    def test_export_refuses_unapproved_or_missing_game_build(self):
        with self.assertRaises(PermissionError):
            self.server.call("noe_export_pack", {"preset":"Linux/X11"})
        with self.assertRaises(ValueError):
            self.server.call("noe_export_pack", {
                "preset":"Linux/X11","approval_token":"approved-secret"
            })
        self.assertFalse((self.root/"builds/Jamila-Game.pck").exists())

    def test_mcp_stdio_protocol(self):
        reqs = [
            {"jsonrpc":"2.0","id":1,"method":"initialize","params":{}},
            {"jsonrpc":"2.0","id":2,"method":"tools/list","params":{}},
            {"jsonrpc":"2.0","id":3,"method":"tools/call",
             "params":{"name":"noe_write_asset","arguments":{
                 "path":"scripts/demo.gd","content":"extends Node\n"}}}
        ]
        stream = "\n".join(json.dumps(r) for r in reqs) + "\n"
        proc = subprocess.run([sys.executable, str(JAMILA/"noe_mcp_server.py"),
                               "--project", str(self.root)],
                              input=stream,text=True,capture_output=True,timeout=7)
        self.assertEqual(proc.returncode, 0, proc.stderr)
        replies = [json.loads(line) for line in proc.stdout.splitlines()]
        self.assertEqual(replies[0]["result"]["serverInfo"]["name"], "jamila-noeengine")
        names={t["name"] for t in replies[1]["result"]["tools"]}
        self.assertTrue({"noe_write_asset","noe_validate_project","noe_export_pack","noe_read_asset"} <= names)
        self.assertTrue(replies[2]["result"]["isError"])
        self.assertFalse((self.root/"scripts/demo.gd").exists())


class JamilaAgentTests(unittest.TestCase):
    def test_plan_allowlist_and_duplicate_rejection(self):
        allowed={"summary":"Generate a scene","actions":[{"tool":"noe_write_asset",
          "arguments":{"path":"scenes/test.tscn","content":"[gd_scene format=3]"}}]}
        self.assertEqual(agent.normalize_plan(allowed)["actions"][0]["tool"],"noe_write_asset")
        with self.assertRaises(ValueError):
            agent.normalize_plan({"summary":"Run bad command","actions":[{"tool":"bash","arguments":{"command":"rm -rf"}}]})
        with self.assertRaises(ValueError):
            agent.normalize_plan({"summary":"Duplicate","actions":[allowed["actions"][0],allowed["actions"][0]]})

    def test_private_api_destination_rejection(self):
        for address in ["http://example.com/v1", "https://127.0.0.1/v1",
                        "https://localhost/v1", "https://192.168.1.2/v1",
                        "https://user:pass@example.com/v1"]:
            with self.subTest(address=address),self.assertRaises(ValueError):
                agent.provider_url(address)
        self.assertEqual(agent.provider_url("https://api.openai.com/v1/chat/completions"),
                         "https://api.openai.com/v1/chat/completions")

    def test_approval_cannot_be_replayed(self):
        with tempfile.TemporaryDirectory() as tmp:
            root=Path(tmp)
            (root/"project.godot").write_text('config_version=5\n')
            local=agent.JamilaService(root, "")
            plan={"summary":"Simple starter","actions":[{"tool":"noe_write_asset",
                "arguments":{"path":"main.gd","content":"extends Node\n","overwrite":False}}]}
            local.plans["once"]={"plan":plan,"expires":1e15}
            output=local.apply({"plan_id":"once"})
            self.assertTrue((root/"main.gd").is_file())
            self.assertFalse(output["ok"]) # No engine binary to validate.
            with self.assertRaises(ValueError):
                local.apply({"plan_id":"once"})


if __name__ == "__main__":
    unittest.main()
