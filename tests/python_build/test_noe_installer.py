"""Installer packaging checks; do not require compiling the full engine."""
import importlib.util
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import unittest
from unittest import mock


SCRIPT = Path(__file__).resolve().parents[2] / "misc/build_products.py"
spec = importlib.util.spec_from_file_location("noe_build_products", SCRIPT)
build = importlib.util.module_from_spec(spec)
spec.loader.exec_module(build)


class InstallerPackagingTests(unittest.TestCase):
    def setUp(self):
        self.directory = tempfile.TemporaryDirectory()
        self.addCleanup(self.directory.cleanup)
        self.root = Path(self.directory.name)
        (self.root / "bin").mkdir()
        (self.root / "Products").mkdir()
        (self.root / "LICENSE.txt").write_text("Upstream MIT test license\n")
        (self.root / "COPYRIGHT.txt").write_text("Upstream copyright test notice\n")
        self.patch_root = mock.patch.object(build, "ROOT", self.root)
        self.patch_products = mock.patch.object(build, "PRODUCTS", self.root / "Products")
        self.patch_root.start()
        self.patch_products.start()
        self.addCleanup(self.patch_root.stop)
        self.addCleanup(self.patch_products.stop)

    def test_binary_selection_keeps_upstream_paths(self):
        binary = self.root / "bin/godot.windows.editor.x86_64.exe"
        binary.write_bytes(b"fixture")
        self.assertEqual(build.find_binary("windows"), binary)
        with self.assertRaises(RuntimeError):
            build.find_binary("linux")

    @unittest.skipUnless(sys.platform.startswith("linux"), "Debian packaging runs on Linux")
    @unittest.skipUnless(shutil.which("dpkg-deb"), "dpkg-deb required")
    def test_deb_setup_contains_launcher_and_attribution(self):
        editor = self.root / "bin/godot.linuxbsd.editor.x86_64"
        shutil.copy2(sys.executable, editor)
        result = build.write_debian(editor)
        self.assertEqual(result.name, "NoeEngine-setup.deb")
        self.assertTrue(result.is_file())
        name = subprocess.check_output(["dpkg-deb", "--field", str(result), "Package"], text=True).strip()
        self.assertEqual(name, "noeengine")
        extract = self.root / "unpacked"
        subprocess.run(["dpkg-deb", "--extract", str(result), str(extract)], check=True)
        self.assertTrue((extract / "usr/lib/noeengine/noeengine").is_file())
        self.assertTrue((extract / "usr/share/doc/noeengine/copyright").is_file())
        self.assertIn("noeengine", (extract / "usr/bin/noeengine").read_text())
        self.assertIn("NoeEngine", (extract / "usr/share/applications/noeengine.desktop").read_text())


if __name__ == "__main__":
    unittest.main()
