"""Strict license-safe cleanup of verbose upstream C++ MIT banners."""
import importlib.util
from pathlib import Path
import unittest

ROOT=Path(__file__).resolve().parents[2]
spec=importlib.util.spec_from_file_location("header_cleaner", ROOT/"misc/compact_godot_headers.py")
module=importlib.util.module_from_spec(spec)
spec.loader.exec_module(module)

SEP = "/" + "*"*74 + "/"
MIT = """/*  area_3d.cpp                                                           */"""
LEGAL = [
 "Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).",
 "Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.",
]
def fixture(lines=None):
    lines = LEGAL if lines is None else lines
    banner=[SEP, MIT, SEP,
            "/*                         This file is part of:                          */",
            "/*                             GODOT ENGINE                               */",
            "/*                        https://godotengine.org                         */",
            SEP]
    banner.extend("/* " + x + " */" for x in lines)
    banner.extend([SEP,
        "/* Permission is hereby granted, free of charge, to any person obtaining a copy */",
        "/* The above copyright notice and this permission notice shall be included */",
        "/* THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND,        */",
        SEP, "", "#include <core.h>", ""])
    return "\n".join(banner)

class BannerTests(unittest.TestCase):
    def test_standard_banner_is_compacted_preserving_both_holders(self):
        output=module.transform(fixture())
        self.assertNotIn("GODOT ENGINE",output)
        self.assertNotIn(SEP,output)
        self.assertTrue(output.startswith("// SPDX-License-Identifier: MIT"))
        for notice in LEGAL:
            self.assertIn(notice,output)
        self.assertIn("#include <core.h>",output)
        self.assertEqual(module.transform(output),output)
    def test_modified_third_party_copyright_is_untouched(self):
        text=fixture(LEGAL + ["Copyright (c) Another Owner"])
        self.assertEqual(module.transform(text),text)
    def test_nonmatching_header_is_untouched(self):
        source=SEP+"\n/* Custom header */\n"+SEP+"\nint main() {}"
        self.assertEqual(module.transform(source),source)
    def test_preserves_crlf(self):
        source=fixture().replace("\n","\r\n")
        updated=module.transform(source)
        self.assertIn("\r\n// Copyright",updated)
        self.assertNotIn("\n\n",updated)
if __name__=="__main__":
    unittest.main()
