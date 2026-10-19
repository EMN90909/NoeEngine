#!/usr/bin/env python3
"""Compare NoeProfiler frame captures without silently mixing monitor and frame rows."""
import argparse
import csv
import json
import math
from pathlib import Path
import statistics
import sys

def read_frames(path):
    with Path(path).open(newline="", encoding="utf-8-sig") as handle:
        rows = csv.reader(handle)
        columns = None
        frames = []
        for row in rows:
            names = [cell.strip() for cell in row]
            if "frame_number" in names and "frame_time_s" in names:
                columns = {name: index for index, name in enumerate(names)}
                continue
            if columns is None or not row or not any(row):
                continue
            try:
                value = row[columns["frame_time_s"]].strip()
                sample_type = row[columns["sample_type"]].strip() if "sample_type" in columns else "frame"
                if sample_type != "frame":
                    continue
                if not value:
                    continue
                milliseconds = float(value) * 1000.0
                if math.isfinite(milliseconds) and milliseconds > 0:
                    frames.append(milliseconds)
            except (ValueError, IndexError):
                # Ignore unrecognized CSV sections following the profiler table.
                continue
    if not frames:
        raise ValueError(f"{path}: no valid individual frame samples in export")
    return frames

def percentile(sorted_values, q):
    position = (len(sorted_values) - 1) * q
    lo = int(math.floor(position))
    hi = int(math.ceil(position))
    return sorted_values[lo] + (sorted_values[hi] - sorted_values[lo]) * (position - lo)

def summarize(frames, spike_ms):
    ordered = sorted(frames)
    return {
        "frames": len(ordered),
        "mean_ms": round(statistics.fmean(ordered), 4),
        "p50_ms": round(percentile(ordered, 0.5), 4),
        "p95_ms": round(percentile(ordered, 0.95), 4),
        "p99_ms": round(percentile(ordered, 0.99), 4),
        "max_ms": round(ordered[-1], 4),
        "spikes": sum(value >= spike_ms for value in ordered),
        "spike_threshold_ms": spike_ms,
    }

def compare(baseline, candidate, max_regression_pct=10.0, spike_ms=33.3, min_frames=5):
    if min(len(baseline), len(candidate)) < min_frames:
        raise ValueError(f"need at least {min_frames} frame samples in both captures")
    a = summarize(baseline, spike_ms)
    b = summarize(candidate, spike_ms)
    percent = ((b["p95_ms"] / a["p95_ms"]) - 1.0) * 100.0
    return {
        "baseline": a,
        "candidate": b,
        "p95_regression_pct": round(percent, 4),
        "max_p95_regression_pct": max_regression_pct,
        "pass": percent <= max_regression_pct,
    }

def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("baseline", type=Path)
    parser.add_argument("candidate", type=Path)
    parser.add_argument("--max-p95-regression-pct", type=float, default=10.0)
    parser.add_argument("--spike-ms", type=float, default=33.3)
    parser.add_argument("--min-frames", type=int, default=5)
    parser.add_argument("--json", type=Path, dest="json_path")
    args = parser.parse_args(argv)
    if not math.isfinite(args.max_p95_regression_pct) or args.max_p95_regression_pct < 0:
        parser.error("--max-p95-regression-pct must be a finite nonnegative number")
    if not math.isfinite(args.spike_ms) or args.spike_ms <= 0 or args.min_frames < 1:
        parser.error("spike threshold and minimum frame count must be positive")
    try:
        report = compare(read_frames(args.baseline), read_frames(args.candidate),
                         args.max_p95_regression_pct, args.spike_ms, args.min_frames)
    except (OSError, ValueError) as exc:
        parser.error(str(exc))
    output = json.dumps(report, indent=2, sort_keys=True)
    print(output)
    if args.json_path:
        args.json_path.write_text(output + "\n", encoding="utf-8")
    return 0 if report["pass"] else 1

if __name__ == "__main__":
    sys.exit(main())
