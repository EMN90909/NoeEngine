// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef MONO_PATH_UTILS_H
#define MONO_PATH_UTILS_H

#include "core/string/string_builder.h"
#include "core/string/ustring.h"

namespace path {

String find_executable(const String &p_name);

String join(const String &p_a, const String &p_b);
String join(const String &p_a, const String &p_b, const String &p_c);
String join(const String &p_a, const String &p_b, const String &p_c, const String &p_d);

/// Returns a normalized absolute path to the current working directory
String cwd();

/**
 * Obtains a normalized absolute path to p_path. Symbolic links are
 * not resolved. The path p_path might not exist in the file system.
 */
String abspath(const String &p_path);

/**
 * Obtains a normalized path to p_path with symbolic links resolved.
 * The resulting path might be either a relative or an absolute path.
 */
String realpath(const String &p_path);

String relative_to(const String &p_path, const String &p_relative_to);
} // namespace path

#endif // MONO_PATH_UTILS_H
