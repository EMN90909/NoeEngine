// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#define DOCTEST_CONFIG_IMPLEMENT
#include "test_macros.h"

HashMap<String, TestFunc> *test_commands = nullptr;

int register_test_command(String p_command, TestFunc p_function) {
	if (!test_commands) {
		test_commands = new HashMap<String, TestFunc>;
	}
	test_commands->insert(p_command, p_function);
	return 0;
}
