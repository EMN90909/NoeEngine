"""Regression tests for real-render benchmark runner and screenshot comparator."""
import importlib.util
from pathlib import Path
import tempfile
import unittest

ROOT=Path(__file__).resolve().parents[1]
PATH=ROOT/"tests"/"noe_render_benchmark"
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
    def test_source_code_exposes_screenshot_requirement(self):
        source=(PATH/"runner.py").read_text()
        self.assertIn("render.png",source)
        self.assertIn("not screenshot.exists()",source)
    def test_performance_comparison_requires_matched_device_driver_scene(self):
        compare=load("noe_render_compare_metrics",PATH/"compare_metrics.py")
        common=dict(renderer_requested="gl_compatibility",resolution_requested="1280x720",
                    scene_source_sha256="abc",device_tag="Intel 2500",driver_tag="Mesa 25",
                    gpu_profiler_requested=False,software_renderer_warning=False)
        before=dict(common,frame_ms_p95=20)
        after=dict(common,frame_ms_p95=18)
        report=compare.compare(before,after)
        self.assertEqual(report["metrics"]["frame_ms_p95"]["percent_change"],-10.0)
        with self.assertRaises(ValueError):
            compare.compare(before,dict(after,driver_tag="unknown"))
        with self.assertRaises(ValueError):
            compare.compare(before,dict(after,device_tag=""))
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
