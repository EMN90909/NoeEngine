import csv
import importlib.util
from pathlib import Path
import tempfile
import unittest

SPEC = importlib.util.spec_from_file_location("noe_profiler_compare", Path(__file__).with_name("compare.py"))
compare_module = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(compare_module)

class CaptureTests(unittest.TestCase):
    def write_csv(self, path, seconds, with_aggregate=True):
        with open(path, "w", newline="", encoding="utf-8") as out:
            w = csv.writer(out)
            w.writerow(["time/fps", "memory/static"])
            w.writerow(["60", "100"])
            w.writerow([])
            w.writerow(["frame_number", "frame_time_s", "sample_type", "spike_over_threshold"])
            for index, value in enumerate(seconds):
                w.writerow([index, value, "frame", "0"])
            if with_aggregate:
                w.writerow(["999", "999.0", "aggregate", "1"])

    def test_parses_mixed_monitor_and_profiler_exports(self):
        with tempfile.TemporaryDirectory() as d:
            path = Path(d) / "a.csv"
            self.write_csv(path, [0.016, 0.017, 0.045])
            self.assertEqual(compare_module.read_frames(path), [16.0, 17.0, 45.0])

    def test_flags_p95_regression(self):
        base = [16.0] * 20
        newer = [24.0] * 20
        self.assertFalse(compare_module.compare(base, newer)["pass"])
        self.assertTrue(compare_module.compare(base, base)["pass"])

    def test_percentiles_and_spikes(self):
        stats = compare_module.summarize([10.0, 15.0, 20.0, 45.0], 33.3)
        self.assertEqual(stats["spikes"], 1)
        self.assertEqual(stats["max_ms"], 45.0)
        self.assertEqual(stats["p50_ms"], 17.5)

    def test_rejects_empty_or_insufficient_capture(self):
        with tempfile.TemporaryDirectory() as d:
            path = Path(d) / "empty.csv"
            path.write_text("time/fps\n60\n", encoding="utf-8")
            with self.assertRaises(ValueError):
                compare_module.read_frames(path)
        with self.assertRaises(ValueError):
            compare_module.compare([16.0], [20.0])

if __name__ == "__main__":
    unittest.main()
