"""Fast build, layout and signed-updater regression checks without native compilation."""
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[2]


class NoeEngineFastRegressionTests(unittest.TestCase):
    def test_editor_icons_have_bounded_intrinsic_size(self):
        for name, dimension in {
            "Godot.svg": 24, "Logo.svg": 32, "GodotFile.svg": 20,
            "DefaultProjectIcon.svg": 64, "GodotMonochrome.svg": 24,
        }.items():
            svg = (ROOT / "editor/icons" / name).read_text()
            self.assertIn(f'width="{dimension}" height="{dimension}"', svg)
        account = (ROOT / "editor/noe_account_workspace.cpp").read_text()
        self.assertIn('get_theme_icon(SNAME("Logo"), SNAME("EditorIcons"))', account)
        self.assertNotIn('get_theme_icon(SNAME("NoeViper"), SNAME("EditorIcons"))', account)

    def test_build_has_independent_fast_editor_path(self):
        bash = (ROOT / "build.sh").read_text()
        py = (ROOT / "misc/build_products.py").read_text()
        self.assertIn('if [[ "$target" == fast ]]; then', bash)
        self.assertIn('python3 misc/build_products.py linux --fast', bash)
        self.assertIn('if args.fast:', py)
        self.assertIn('prefer_dev=True', py)
        self.assertIn('args.extend(["dev_build=yes", "production=no", "lto=none"])', py)
        self.assertIn('wrapper_dir / "g++"', py)
        self.assertIn('link.symlink_to(shutil.which("ccache"))', py)

    def test_product_version_is_consistent(self):
        version = (ROOT / "misc/noe_product_version.txt").read_text().strip()
        header = (ROOT / "editor/noe_product_version.h").read_text()
        native = (ROOT / "editor/noe_update_manager.cpp").read_text()
        package = (ROOT / "misc/build_products.py").read_text()
        self.assertIn(f'#define NOE_PRODUCT_VERSION "{version}"', header)
        self.assertIn('#include "editor/noe_product_version.h"', native)
        self.assertIn('PRODUCT_VERSION.replace("-", "~")', package)

    def test_signed_update_flow_can_restart_from_project_manager(self):
        manager = (ROOT / "editor/project_manager.cpp").read_text()
        updates = (ROOT / "editor/noe_update_manager.cpp").read_text()
        packager = (ROOT / "misc/build_products.py").read_text()
        self.assertIn('NoeUpdateManager::install(this, hb);', manager)
        self.assertIn('FileAccess::get_sha256(saved_path).to_lower()', updates)
        self.assertIn('return base.path_join("noe-updater");', updates)
        self.assertIn('stage_updater(exe.parent)', packager)
        self.assertIn('stage_updater(app)', packager)


if __name__ == "__main__":
    unittest.main()
