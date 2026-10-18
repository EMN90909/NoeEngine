"""No-privilege distro bootstrap tests, including the antiX eudev conflict."""
from pathlib import Path
import importlib.util
import tempfile
import unittest
from unittest import mock

SOURCE = Path(__file__).resolve().parents[2] / "misc/bootstrap_build_deps.py"
spec = importlib.util.spec_from_file_location("noe_bootstrap_build_deps", SOURCE)
bootstrap = importlib.util.module_from_spec(spec)
spec.loader.exec_module(bootstrap)


class BuildDependencyBootstrapTests(unittest.TestCase):
    def test_antix_identity_variants(self):
        examples = (
            'NAME="antiX"\nID=antix\nID_LIKE=debian\n',
            'PRETTY_NAME="antiX 26.1"\nID=debian\n',
        )
        with tempfile.TemporaryDirectory() as tmp:
            file = Path(tmp) / "os-release"
            marker = Path(tmp) / "antix-version"
            for value in examples:
                file.write_text(value, encoding="utf-8")
                self.assertTrue(bootstrap.is_antix_system(file, marker))
            file.write_text('ID=debian\nNAME="Debian GNU/Linux"\n', encoding="utf-8")
            self.assertFalse(bootstrap.is_antix_system(file, marker))
            marker.write_text("antiX-26.1_x64-full (Debian trixie)\n", encoding="utf-8")
            self.assertTrue(bootstrap.is_antix_system(file, marker))
            marker.write_text("not an antiX marker\n", encoding="utf-8")
            self.assertFalse(bootstrap.is_antix_system(file, marker))
            marker.unlink()
            file.write_text('ID=devuan\nPRETTY_NAME="Devuan GNU/Linux"\n', encoding="utf-8")
            self.assertFalse(bootstrap.is_antix_system(file, marker))
            file.unlink()
            self.assertFalse(bootstrap.is_antix_system(file, marker))

    def test_antix_linux_bootstrap_skips_conflicting_udev(self):
        packages = bootstrap.linux_build_packages(["linux"], antix=True)
        self.assertIn("libx11-dev", packages)
        self.assertIn("libpulse-dev", packages)
        self.assertNotIn("libudev-dev", packages)
        self.assertNotIn("libeudev-dev", packages)
        self.assertEqual(len(packages), len(set(packages)))

    def test_normal_debian_does_not_force_optional_udev_package(self):
        packages = bootstrap.linux_build_packages(["linux"], antix=False)
        self.assertNotIn("libudev-dev", packages)
        self.assertIn("libx11-dev", packages)

    def test_linux_package_selection_across_distros(self):
        # On any apt-based distro, optional hotplug must not block the build.
        for antix in (True, False):
            for targets in (["linux"], ["linux", "windows"], ["linux", "android"]):
                with self.subTest(antix=antix, targets=targets):
                    packages = bootstrap.linux_build_packages(targets, antix=antix)
                    self.assertNotIn("libudev-dev", packages)
                    self.assertNotIn("libeudev-dev", packages)
                    self.assertEqual(len(packages), len(set(packages)))

    def test_other_platform_build_requirements_unchanged(self):
        packages = bootstrap.linux_build_packages(
            ["linux", "windows", "android"], antix=True
        )
        for name in ("mingw-w64", "nsis", "openjdk-17-jdk", "unzip", "curl"):
            self.assertIn(name, packages)
        self.assertNotIn("libudev-dev", packages)

    @mock.patch.object(bootstrap, "is_antix_system", return_value=True)
    @mock.patch.object(bootstrap.platform, "system", return_value="Linux")
    @mock.patch.object(bootstrap.os, "geteuid", return_value=0)
    def test_simulated_install_calls_apt_without_udev(self, *_):
        with mock.patch.object(bootstrap.shutil, "which", return_value="/usr/bin/apt-get"), \
                mock.patch.object(bootstrap.subprocess, "run") as run:
            bootstrap.install_distro_packages(["linux"], dry_run=False)
        commands = [call.args[0] for call in run.call_args_list]
        self.assertEqual(len(commands), 2)
        self.assertEqual(commands[0], ["apt-get", "update"])
        self.assertEqual(commands[1][:4], ["apt-get", "install", "-y", "--no-install-recommends"])
        self.assertNotIn("libudev-dev", commands[1])
        self.assertNotIn("libeudev-dev", commands[1])

    @mock.patch.object(bootstrap, "is_antix_system", return_value=True)
    @mock.patch.object(bootstrap.platform, "system", return_value="Linux")
    @mock.patch.object(bootstrap.os, "geteuid", return_value=0)
    def test_dry_run_never_calls_apt(self, *_):
        with mock.patch.object(bootstrap.shutil, "which", return_value="/usr/bin/apt-get"), \
                mock.patch.object(bootstrap.subprocess, "run") as run:
            bootstrap.install_distro_packages(["linux"], dry_run=True)
        run.assert_not_called()


if __name__ == "__main__":
    unittest.main()
