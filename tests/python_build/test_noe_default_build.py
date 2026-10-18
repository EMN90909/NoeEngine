"""Ensure plain ./build.sh orchestrates the requested full build, not Linux-only.

Static test: avoids installing packages or compiling the engine in CI.
"""
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[2]
SCRIPT = (ROOT / "build.sh").read_text(encoding="utf-8")

class DefaultBuildPipelineTests(unittest.TestCase):
    def test_four_default_stages_are_ordered(self):
        markers = [
            '"$ROOT/build.sh" check',
            '"$ROOT/build.sh" linux 2 --install-deps',
            '"$ROOT/build.sh" windows 2 --install-deps',
            '"$ROOT/build.sh" android 2 --install-deps'
        ]
        positions = [SCRIPT.index(marker) for marker in markers]
        self.assertEqual(positions, sorted(positions))
        self.assertIn('if [[ $# -eq 0 ]]; then', SCRIPT)
        self.assertIn('set -Eeuo pipefail', SCRIPT)

    def test_explicit_target_still_works(self):
        self.assertIn('target="${1:-linux}"', SCRIPT)
        self.assertIn('linux|windows|macos|android|all|check|patch-tools', SCRIPT)
        self.assertIn('python3 misc/build_products.py "$platform" --preflight', SCRIPT)

    def test_installation_is_explicit_and_fails_closed(self):
        self.assertIn('if [[ "$install_deps" == 1 ]]; then', SCRIPT)
        self.assertIn('misc/bootstrap_build_deps.py', SCRIPT)
        self.assertIn('if [[ ! -s "$artifact" ]]; then', SCRIPT)

if __name__ == "__main__":
    unittest.main()
