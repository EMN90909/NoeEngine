// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "tests/test_utils.h"

#include "core/os/os.h"

String TestUtils::get_data_path(const String &p_file) {
	String data_path = "../tests/data";
	return get_executable_dir().path_join(data_path.path_join(p_file));
}

String TestUtils::get_executable_dir() {
	return OS::get_singleton()->get_executable_path().get_base_dir();
}
