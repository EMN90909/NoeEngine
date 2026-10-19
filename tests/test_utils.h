// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef TEST_UTILS_H
#define TEST_UTILS_H

class String;

namespace TestUtils {

String get_data_path(const String &p_file);
String get_executable_dir();
} // namespace TestUtils

#endif // TEST_UTILS_H
