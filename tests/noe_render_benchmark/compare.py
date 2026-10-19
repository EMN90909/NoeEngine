#!/usr/bin/env python3
"""Per-device NoeRender visual regression comparer (Pillow required).

Reference screenshots must match the same GPU, driver, viewport, renderer,
resolution, scene commit and graphics settings. Never silently bless a new
reference. Designed for controlled graphics QA, not synthetic headless checks.
"""
import argparse
import json
from pathlib import Path
from PIL import Image, ImageChops, ImageStat

def compare(reference, candidate, threshold):
    with Image.open(reference) as expected, Image.open(candidate) as actual:
        if expected.size != actual.size:
            raise ValueError("Screenshot dimensions do not match")
        delta = ImageChops.difference(expected.convert("RGB"), actual.convert("RGB"))
        stats = ImageStat.Stat(delta)
        mean = sum(stats.mean) / 3
        peak = max(extrema[1] for extrema in delta.getextrema())
        return {"mean_absolute_channel_difference": round(mean,4),
                "peak_channel_difference": peak, "passed": mean <= threshold,
                "width": expected.width, "height": expected.height}
def main():
    p=argparse.ArgumentParser()
    p.add_argument("--reference",type=Path,required=True)
    p.add_argument("--actual",type=Path,required=True)
    p.add_argument("--threshold",type=float,default=5.0)
    p.add_argument("--report",type=Path)
    a=p.parse_args()
    result=compare(a.reference,a.actual,a.threshold)
    print(json.dumps(result,indent=2))
    if a.report: a.report.write_text(json.dumps(result,indent=2)+"\n",encoding="utf8")
    if not result["passed"]: raise SystemExit(1)
if __name__=="__main__":main()
