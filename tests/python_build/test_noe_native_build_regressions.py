"""Guard native build regressions that source-only preflight previously missed."""
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[2]


class NativeBuildRegressionTests(unittest.TestCase):
    def test_http_response_byte_array_uses_engine_utf8_api(self):
        source_header = (ROOT / "core/string/ustring.h").read_text(encoding="utf-8")
        self.assertIn("static String utf8(const char *p_utf8, int p_len = -1)", source_header)
        for file, count in (
            ("editor/noe_account_workspace.cpp", 4),  # Includes the new authenticated bridge response.
            ("editor/noe_update_manager.cpp", 1),
        ):
            with self.subTest(file=file):
                source = (ROOT / file).read_text(encoding="utf-8")
                self.assertNotIn("body.get_string_from_utf8()", source)
                self.assertEqual(
                    source.count('String::utf8(reinterpret_cast<const char *>(body.ptr()), body.size())'),
                    count,
                )

    def test_android_ndk_pins_match_in_all_build_inputs(self):
        detect = (ROOT / "platform/android/detect.py").read_text(encoding="utf-8")
        config = (ROOT / "platform/android/java/app/config.gradle").read_text(encoding="utf-8")
        bootstrap = (ROOT / "misc/bootstrap_build_deps.py").read_text(encoding="utf-8")
        build = (ROOT / "misc/build_products.py").read_text(encoding="utf-8")
        ndk = "23.2.8568313"
        self.assertIn('return "' + ndk + '"', detect)
        self.assertIn("ndkVersion         : '" + ndk + "'", config)
        self.assertIn('"ndk;' + ndk + '"', bootstrap)
        self.assertIn('ndk/' + ndk, build)
        self.assertIn('"build-tools;33.0.2"', bootstrap)
        self.assertIn('"platforms;android-33"', bootstrap)

    def test_android_build_packages_real_apk_with_gradle(self):
        build = (ROOT / "misc/build_products.py").read_text(encoding="utf-8")
        self.assertIn('command(["./gradlew", "generateGodotEditor", "--no-daemon"]', build)
        self.assertIn('os.environ["ANDROID_SDK_ROOT"]', build)
        self.assertIn('find_android_editor_apk()', build)


if __name__ == "__main__":
    unittest.main()
