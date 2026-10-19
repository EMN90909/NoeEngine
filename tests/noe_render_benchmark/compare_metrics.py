#!/usr/bin/env python3
"""Compare matched NoeRender performance captures, without pretending GPU validation.

Requires explicit identical device and driver tags supplied by the test operator.
This is a change-detection tool; measurements are real only if runner.py was
executed on a physical GPU with an actual graphics display.
"""
from __future__ import annotations
import argparse
import json
from pathlib import Path

REQUIRED = ("renderer_requested", "resolution_requested", "scene_source_sha256",
            "device_tag", "driver_tag", "gpu_profiler_requested",
            "software_renderer_warning")
METRICS = ("frame_ms_p50", "frame_ms_p95", "frame_ms_p99",
           "draw_calls_last_frame", "video_memory_estimate_bytes")

def compare(before: dict, after: dict) -> dict:
    for name in REQUIRED:
        if name not in before or name not in after:
            raise ValueError(f"Missing required comparison metadata: {name}")
        if name in ("device_tag", "driver_tag") and (not before[name] or not after[name]):
            raise ValueError(f"Hardware comparison requires recorded {name}")
        if before[name] != after[name]:
            raise ValueError(f"Mismatch in {name}: {before[name]!r} vs {after[name]!r}")
    if before["software_renderer_warning"]:
        raise ValueError("Software rasterizer samples are not a physical-GPU comparison")
    result = {"device_tag": before["device_tag"], "driver_tag": before["driver_tag"],
              "renderer": before["renderer_requested"], "scene_source_sha256": before["scene_source_sha256"],
              "metrics": {}, "warning": "Descriptive frame deltas only; no GPU timestamp or statistical significance guarantee"}
    for name in METRICS:
        left = before.get(name)
        right = after.get(name)
        if left is None or right is None:
            continue
        left, right = float(left), float(right)
        if left < 0.0 or right < 0.0:
            raise ValueError(f"Invalid negative metric: {name}")
        result["metrics"][name] = {
            "before": left, "after": right,
            "percent_change": round((right-left) / left * 100.0, 3) if left else None,
        }
    return result

def main(argv=None):
    p=argparse.ArgumentParser()
    p.add_argument("--before",type=Path,required=True)
    p.add_argument("--after",type=Path,required=True)
    p.add_argument("--report",type=Path)
    a=p.parse_args(argv)
    result=compare(json.loads(a.before.read_text()),json.loads(a.after.read_text()))
    if a.report:
        a.report.parent.mkdir(parents=True,exist_ok=True)
        a.report.write_text(json.dumps(result,indent=2)+"\n")
    print(json.dumps(result,indent=2))
if __name__=="__main__":
    main()
