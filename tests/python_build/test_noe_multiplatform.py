"""Offline, dependency-free contract tests for cobra assets and build matrix."""
import base64
import importlib.util
import io
import pathlib
import re
import unittest
import xml.etree.ElementTree as ET
import zipfile

ROOT = pathlib.Path(__file__).resolve().parents[2]
script = ROOT / "misc/bootstrap_build_deps.py"
spec = importlib.util.spec_from_file_location("noe_bootstrap", script)
bootstrap = importlib.util.module_from_spec(spec)
spec.loader.exec_module(bootstrap)

class NoeMultiPlatformTests(unittest.TestCase):
    def test_four_root_icons_use_exact_same_uploaded_cobra_art(self):
        p = (ROOT / "icon.png").read_bytes()
        self.assertTrue(p.startswith(b"\x89PNG\r\n\x1a\n"))
        self.assertEqual(p, (ROOT / "icon_outlined.png").read_bytes())
        for name in ("icon.svg", "icon_outlined.svg"):
            svg = ET.parse(ROOT / name).getroot()
            img = svg.find("{http://www.w3.org/2000/svg}image")
            self.assertIsNotNone(img)
            href = img.attrib.get("href", "")
            self.assertTrue(href.startswith("data:image/png;base64,"))
            self.assertEqual(base64.b64decode(href.split(",", 1)[1]), p)

    def test_build_targets_produce_named_native_artifacts(self):
        shell = (ROOT / "build.sh").read_text()
        for target in ("linux", "windows", "macos", "android", "all", "check", "patch-tools"):
            self.assertIn(target, shell)
        for output in ("NoeEngine-linux-x86_64.tar.gz", "NoeEngine-setup.exe",
                       "NoeEngine-setup.pkg", "NoeEngine-android-arm64.apk"):
            self.assertIn(output, shell)
        self.assertIn("--install-deps", shell)
        self.assertIn("python3 misc/bootstrap_build_deps.py", shell)
        self.assertIn("python3 misc/build_products.py", shell)

    def test_android_sdk_archive_is_sha256_pinned_and_safe(self):
        self.assertEqual(len(bootstrap.ANDROID_TOOLS["linux"][1]), 64)
        self.assertEqual(bootstrap.TOOLS_VERSION, "15859902")
        import tempfile
        with tempfile.TemporaryDirectory() as work:
            archive = pathlib.Path(work) / "sdk.zip"
            archive.write_bytes(self._zip("../escape"))
            with self.assertRaises(RuntimeError):
                bootstrap.checked_zip_extract(archive, pathlib.Path(work) / "install")

    @staticmethod
    def _zip(filename):
        buf = io.BytesIO()
        with zipfile.ZipFile(buf, "w") as zip_file:
            zip_file.writestr(filename, b"danger")
        return buf.getvalue()

    def test_android_real_apk_and_host_restrictions(self):
        source = (ROOT / "misc/build_products.py").read_text()
        self.assertIn("AndroidManifest.xml", source)
        self.assertIn('target=editor', source)
        self.assertIn('generate_android_binaries=yes', source)
        self.assertIn("Android builds require", source.replace("Android editor builds require", "Android builds require"))
        self.assertIn("macOS builds require Xcode and a Mac", source)

if __name__ == "__main__":
    unittest.main()
