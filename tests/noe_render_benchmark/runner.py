#!/usr/bin/env python3
"""Run a real *rendered* NoeRender benchmark, collect logs and optional baseline.

Headless rendering is explicitly rejected. Software rasterizers can run but
are labeled as software and never count as physical GPU validation.
"""
from __future__ import annotations
import argparse
import json
import os
from pathlib import Path
import platform
import re
import subprocess
import sys
import time

ROOT = Path(__file__).resolve().parents[2]
PROJECT = ROOT / "tests" / "noe_render_benchmark"
MARKER = "NOERENDER_BENCHMARK:"

def parse_result(output: str) -> dict:
    for line in output.splitlines():
        if MARKER in line:
            return json.loads(line.split(MARKER, 1)[1].strip())
    raise RuntimeError("No benchmark marker in engine stdout. Inspect log for scene errors.")

def run_benchmark(engine: str, output_dir: Path, renderer: str, timeout: int = 180) -> dict:
    output_dir.mkdir(parents=True, exist_ok=True)
    screenshot = output_dir / "render.png"
    environment = os.environ.copy()
    environment["NOERENDER_SCREENSHOT_PATH"] = str(screenshot.resolve())
    cmd = [engine, "--path", str(PROJECT), "--rendering-method", renderer,
           "--resolution", "1280x720", "--windowed"]
    if "--headless" in cmd or os.environ.get("NOERENDER_HEADLESS") == "1":
        raise RuntimeError("Benchmark requires a graphics display; --headless is not valid.")
    start = time.monotonic()
    proc = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True,
                          env=environment, timeout=timeout, check=False)
    elapsed = time.monotonic() - start
    log = proc.stdout + "\n" + proc.stderr
    (output_dir / "engine.log").write_text(log, encoding="utf-8")
    if proc.returncode:
        raise RuntimeError(f"Engine exited {proc.returncode}. See {output_dir / 'engine.log'}")
    metrics = parse_result(log)
    if not screenshot.exists() or screenshot.stat().st_size < 100:
        raise RuntimeError("No real rendered screenshot captured. Headless or GPU init failed.")
    metrics.update({"host_os": platform.platform(), "renderer_requested": renderer,
                    "duration_wall_seconds": round(elapsed, 3),
                    "rendered_screenshot": str(screenshot),
                    "software_renderer_warning": "llvmpipe" in log.lower() or "softpipe" in log.lower()})
    (output_dir / "metrics.json").write_text(json.dumps(metrics, indent=2) + "\n", encoding="utf-8")
    return metrics

def main(argv=None):
    parser=argparse.ArgumentParser()
    parser.add_argument("--engine", required=True, help="compiled NoeEngine executable")
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--renderer", choices=("gl_compatibility","forward_plus","mobile"), default="gl_compatibility")
    parser.add_argument("--timeout", type=int, default=180)
    args=parser.parse_args(argv)
    print(json.dumps(run_benchmark(args.engine, args.output, args.renderer, args.timeout), indent=2))
if __name__=="__main__":
    sys.exit(main())
