"""Regression tests for real-render benchmark runner and screenshot comparator."""
import importlib.util
from pathlib import Path
import tempfile
import unittest

ROOT=Path(__file__).resolve().parents[1]
PATH=ROOT/"noe_render_benchmark"
def load(name,path):
    spec=importlib.util.spec_from_file_location(name,path)
    module=importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module
runner=load("noe_render_runner",PATH/"runner.py")

class BenchmarkHarnessTests(unittest.TestCase):
    def test_requires_marker(self):
        with self.assertRaises(RuntimeError): runner.parse_result("no benchmark")
    def test_parses_metrics(self):
        metrics=runner.parse_result('NOERENDER_BENCHMARK:{"frames":240,"frame_ms_p95":21.0}')
        self.assertEqual(metrics["frames"],240)
    def test_no_headless_forgery(self):
        with tempfile.TemporaryDirectory() as path:
            import os
            old=os.environ.get("NOERENDER_HEADLESS")
            os.environ["NOERENDER_HEADLESS"]="1"
            try:
                with self.assertRaisesRegex(RuntimeError,"display"):
                    runner.run_benchmark("missing-engine",Path(path),"gl_compatibility")
            finally:
                if old is None: os.environ.pop("NOERENDER_HEADLESS",None)
                else: os.environ["NOERENDER_HEADLESS"]=old
if __name__=="__main__": unittest.main()
