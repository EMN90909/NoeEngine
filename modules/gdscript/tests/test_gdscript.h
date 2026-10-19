// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef TEST_GDSCRIPT_H
#define TEST_GDSCRIPT_H

#include "gdscript_test_runner.h"
#include "tests/test_macros.h"

namespace GDScriptTests {

enum TestType {
	TEST_TOKENIZER,
	TEST_PARSER,
	TEST_COMPILER,
	TEST_BYTECODE,
};

void test(TestType p_type);

} // namespace GDScriptTests

#endif // TEST_GDSCRIPT_H
