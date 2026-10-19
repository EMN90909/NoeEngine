// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef CORE_CONSTANTS_H
#define CORE_CONSTANTS_H

#include "core/string/string_name.h"
#include "core/templates/hash_set.h"

class CoreConstants {
public:
	static int get_global_constant_count();
	static StringName get_global_constant_enum(int p_idx);
	static bool is_global_constant_bitfield(int p_idx);
	static bool get_ignore_value_in_docs(int p_idx);
	static const char *get_global_constant_name(int p_idx);
	static int64_t get_global_constant_value(int p_idx);
	static bool is_global_constant(const StringName &p_name);
	static int get_global_constant_index(const StringName &p_name);
	static bool is_global_enum(const StringName &p_enum);
	static void get_enum_values(StringName p_enum, HashMap<StringName, int64_t> *p_values);
};

#endif // CORE_CONSTANTS_H
