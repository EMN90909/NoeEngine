#!/usr/bin/env python3
"""Jamila local agent: model plan -> user approval -> real NoeEngine MCP calls.

Python stdlib only. The endpoint listens on loopback and demands a per-launch
random bearer. The model can suggest files but cannot execute arbitrary shell.
Adapted from Jamila-GD's game-agent stages and NoeEngine project MCP.
"""
import argparse
import ipaddress
import json
import os
from pathlib import Path
import secrets
import subprocess
import sys
import threading
import time
import urllib.error
import urllib.parse
import urllib.request
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer

MAX_REQUEST = 130_000
MAX_FILES = 8
SYSTEM = """You are Jamila, a NoeEngine game-building assistant ported from Jamila-GD.
Work in ordered specialist stages: plan, scene, gameplay, integration, QA.
Return ONLY a JSON object: {"summary":"what will be built","actions":[{"tool":"noe_write_asset","arguments":{"path":"scenes/main.tscn","content":"full literal UTF-8 file content","overwrite":false}}, ...]}.
Allowed action tools: noe_write_asset only. The local agent automatically runs
noe_validate_project after applying approved writes. Never invent success or
claim that a game was tested. Create a small playable slice and keep existing
assets untouched unless the user explicitly requested an overwrite.
Scene files must be valid NoeEngine/Godot .tscn text; GDScript scripts must match
scene node paths. For a runnable demo use res:// scripts and suitable scene names.
Return at most 8 full files, each <= 96 kB; no shell commands, paths outside the
project, remote downloads, secrets, installers, or network tools.
The user must review and explicitly approve ALL file edits before execution."""

class Bridge:
    def __init__(self, project, engine):
        self.project = str(Path(project).resolve())
        self.engine = str(Path(engine).resolve()) if engine else ""
        self.script = str(Path(__file__).with_name("noe_mcp_server.py"))
        self.approval = secrets.token_hex(32)

    def call(self, name, args=None, approved=False):
        payload = dict(args or {})
        if approved:
            payload["approval_token"] = self.approval
        env = os.environ.copy()
        env["JAMILA_APPROVAL_SECRET"] = self.approval
        request = {"jsonrpc":"2.0", "id":1, "method":"tools/call",
                   "params":{"name":name,"arguments":payload}}
        run = subprocess.run([sys.executable, "-u", self.script, "--project", self.project,
                              "--engine", self.engine], input=json.dumps(request)+"\n",
                             text=True, capture_output=True, timeout=105, env=env)
        if run.returncode != 0:
            raise RuntimeError("MCP server unavailable: " + run.stderr[-250:])
        result = json.loads(run.stdout.splitlines()[-1])["result"]
        content = result.get("content", [{}])[0].get("text","")
        if result.get("isError"):
            raise ValueError(content)
        return json.loads(content)

def normalize_plan(output):
    if not isinstance(output, dict) or not isinstance(output.get("summary"), str):
        raise ValueError("Model returned an invalid plan")
    actions = output.get("actions")
    if not isinstance(actions,list) or not 1 <= len(actions) <= MAX_FILES:
        raise ValueError("Model must propose 1 to 8 project files")
    normalized = []
    for item in actions:
        if not isinstance(item,dict) or item.get("tool")!="noe_write_asset":
            raise ValueError("Only approved project file writes are supported")
        args=item.get("arguments")
        if not isinstance(args,dict):
            raise ValueError("Missing write arguments")
        path=args.get("path")
        content=args.get("content")
        if not isinstance(path,str) or not path or len(path)>320 or not isinstance(content,str) or len(content.encode())>96000:
            raise ValueError("Invalid game asset proposal")
        normalized.append({"tool":"noe_write_asset","arguments":{
            "path":path,"content":content,"overwrite":args.get("overwrite") is True
        }})
    if len({a["arguments"]["path"] for a in normalized}) != len(normalized):
        raise ValueError("Duplicate asset paths in plan")
    return {"summary":output["summary"][:1200], "actions":normalized}

def provider_url(value):
    if not isinstance(value,str) or len(value)>400:
        raise ValueError("Invalid AI provider URL")
    url=urllib.parse.urlparse(value)
    if url.scheme!="https" or not url.hostname or url.username or url.password or url.fragment:
        raise ValueError("Use an HTTPS model endpoint")
    if url.port and url.port!=443:
        raise ValueError("Use HTTPS port 443")
    try:
        ip=ipaddress.ip_address(url.hostname)
        if not ip.is_global:
            raise ValueError("Private model IP destinations are not supported")
    except ValueError as exc:
        if "Private model" in str(exc):
            raise
    if url.hostname in ("localhost", "localhost.localdomain") or url.hostname.endswith(".local"):
        raise ValueError("Local model endpoints need a separate trusted adapter")
    return value

def call_model(url, api_key, model, prompt, context):
    if not isinstance(api_key,str) or not 8<=len(api_key)<=4096:
        raise ValueError("Provide the model API key")
    if not isinstance(model,str) or not 1<=len(model)<=128:
        raise ValueError("Provide the model name")
    url=provider_url(url)
    body=json.dumps({"model":model,"temperature":0.2,"max_tokens":6500,
        "messages":[{"role":"system","content":SYSTEM},
                    {"role":"user","content":json.dumps({"request":prompt,"project_context":context})}]}).encode()
    req=urllib.request.Request(url,body,method="POST",headers={
        "Authorization":"Bearer "+api_key,"Content-Type":"application/json","Accept":"application/json"
    })
    try:
        with urllib.request.urlopen(req,timeout=48) as response:
            raw=response.read(600_000)
    except (urllib.error.URLError,TimeoutError) as exc:
        raise RuntimeError("Model request failed") from exc
    data=json.loads(raw)
    message=data["choices"][0]["message"]["content"]
    if isinstance(message,list):
        message="".join(x.get("text","") for x in message if isinstance(x,dict))
    if not isinstance(message,str):
        raise ValueError("Model did not return a text plan")
    message=message.strip()
    if message.startswith("```"):
        message=message.partition("\n")[2].rsplit("```",1)[0].strip()
    return normalize_plan(json.loads(message))

class JamilaService:
    def __init__(self,project,engine):
        self.bridge=Bridge(project,engine)
        self.plans={}
        self.lock=threading.Lock()
        self.validated=False

    def plan(self,payload):
        self.validated=False
        prompt=payload.get("prompt","")
        if not isinstance(prompt,str) or not 1<=len(prompt.strip())<=3000:
            raise ValueError("Write a game-building task (1 to 3000 characters)")
        info=self.bridge.call("noe_project_info")
        assets=self.bridge.call("noe_list_assets",{"limit":70})
        # Never send project.godot secrets or editor auth settings to the provider.
        context={"project":info,"assets":assets["assets"][:70]}
        model_plan=call_model(payload.get("api_url",""),payload.get("api_key",""),
                              payload.get("model",""),prompt,context)
        # MCP validates proposed paths before the user sees the plan.
        from noe_mcp_server import ProjectTools
        validator=ProjectTools(self.bridge.project,self.bridge.engine)
        for action in model_plan["actions"]:
            path=validator.path(action["arguments"]["path"],writing=True)
            if path.exists() and not action["arguments"]["overwrite"]:
                raise ValueError("Model tried to overwrite an existing asset without approval: "+path.name)
        token=secrets.token_hex(16)
        with self.lock:
            self.plans={k:v for k,v in self.plans.items() if v["expires"]>time.time()}
            self.plans[token]={"plan":model_plan,"expires":time.time()+600}
        return {"plan_id":token, **model_plan}

    def apply(self,payload):
        plan_id=payload.get("plan_id")
        if not isinstance(plan_id,str):
            raise ValueError("Missing approved plan")
        with self.lock:
            selected=self.plans.pop(plan_id,None)
        if not selected or selected["expires"]<time.time():
            raise ValueError("Plan expired; request a new plan")
        outcomes=[]
        for item in selected["plan"]["actions"]:
            try:
                outcome=self.bridge.call(item["tool"],item["arguments"],approved=True)
                outcomes.append({"tool":item["tool"],"path":item["arguments"]["path"],"ok":True,"result":outcome})
            except (ValueError,RuntimeError,subprocess.TimeoutExpired) as exc:
                outcomes.append({"tool":item["tool"],"path":item["arguments"]["path"],"ok":False,"error":str(exc)[:400]})
                break
        if outcomes and all(item["ok"] for item in outcomes):
            try:
                check=self.bridge.call("noe_validate_project",approved=True)
            except (ValueError,RuntimeError,subprocess.TimeoutExpired) as exc:
                check={"ok":False,"error":str(exc)[:400]}
        else:
            check={"ok":False,"error":"Validation skipped because a write failed"}
        self.validated=bool(outcomes and all(x["ok"] for x in outcomes) and check.get("ok"))
        return {"files":outcomes,"validation":check,"ok":self.validated}

    def export_game(self,payload):
        if not self.validated:
            raise ValueError("Create and validate a game before exporting it")
        preset=payload.get("preset")
        if not isinstance(preset,str) or len(preset)>80:
            raise ValueError("Select a valid project export preset")
        # Validation and export are real headless NoeEngine invocations.
        # Both use the same locally approved, constrained MCP toolchain.
        checks=self.bridge.call("noe_validate_project",approved=True)
        if not checks.get("ok"):
            self.validated=False
            return {"ok":False,"validation":checks,"error":"Project validation failed before export"}
        export=self.bridge.call("noe_export_pack",{"preset":preset},approved=True)
        return {"ok":bool(export.get("ok")),"export":export}

def make_handler(service,token):
    class AgentHandler(BaseHTTPRequestHandler):
        def log_message(self, *_):
            pass
        def respond(self,status,data):
            body=json.dumps(data).encode()
            self.send_response(status)
            self.send_header("Content-Type","application/json")
            self.send_header("Cache-Control","no-store")
            self.send_header("X-Content-Type-Options","nosniff")
            self.send_header("Content-Length",str(len(body)))
            self.end_headers()
            self.wfile.write(body)
        def do_GET(self):
            if self.path!="/health":
                return self.respond(404,{"error":"not_found"})
            return self.respond(200,{"ok":True,"mcp":"stdio-jsonrpc","approval":"required"})
        def do_POST(self):
            if self.headers.get("Origin"):
                return self.respond(403,{"error":"browser_cross_origin_forbidden"})
            host=self.headers.get("Host","")
            if host not in (f"127.0.0.1:{self.server.server_port}",f"localhost:{self.server.server_port}"):
                return self.respond(403,{"error":"invalid_host"})
            auth=self.headers.get("Authorization","")
            if not secrets.compare_digest(auth,"Bearer "+token):
                return self.respond(401,{"error":"unauthorized"})
            length=int(self.headers.get("Content-Length","0"))
            if not 0<length<=MAX_REQUEST:
                return self.respond(413,{"error":"request_too_large"})
            try:
                payload=json.loads(self.rfile.read(length))
                if not isinstance(payload,dict):
                    raise ValueError("Request must be an object")
                if self.path=="/v1/plan":
                    output=service.plan(payload)
                elif self.path=="/v1/apply":
                    output=service.apply(payload)
                elif self.path=="/v1/export":
                    output=service.export_game(payload)
                else:
                    return self.respond(404,{"error":"not_found"})
                return self.respond(200,output)
            except (ValueError,KeyError,TypeError,json.JSONDecodeError) as exc:
                return self.respond(400,{"error":str(exc)[:300]})
            except (OSError,RuntimeError,subprocess.TimeoutExpired) as exc:
                return self.respond(502,{"error":str(exc)[:300]})
    return AgentHandler

def main():
    parser=argparse.ArgumentParser()
    parser.add_argument("--project",required=True)
    parser.add_argument("--engine",default="")
    parser.add_argument("--port",type=int,required=True)
    parser.add_argument("--token",required=True)
    args=parser.parse_args()
    if not 1024<=args.port<=65535 or len(args.token)<32:
        return 2
    service=JamilaService(args.project,args.engine)
    httpd=ThreadingHTTPServer(("127.0.0.1",args.port),make_handler(service,args.token))
    httpd.serve_forever(poll_interval=0.5)
    return 0

if __name__=="__main__":
    raise SystemExit(main())
