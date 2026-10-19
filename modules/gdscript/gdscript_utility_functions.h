// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef GDSCRIPT_UTILITY_FUNCTIONS_H
#define GDSCRIPT_UTILITY_FUNCTIONS_H

#include "core/string/string_name.h"
#include "core/variant/variant.h"

template <typename T>
class TypedArray;

class GDScriptUtilityFunctions {
public:
	typedef void (*FunctionPtr)(Variant *r_ret, const Variant **p_args, int p_arg_count, Callable::CallError &r_error);

	static FunctionPtr get_function(const StringName &p_function);
	static bool has_function_return_value(const StringName &p_function);
	static Variant::Type get_function_return_type(const StringName &p_function);
	static StringName get_function_return_class(const StringName &p_function);
	static Variant::Type get_function_argument_type(const StringName &p_function, int p_arg);
	static int get_function_argument_count(const StringName &p_function, int p_arg);
	static bool is_function_vararg(const StringName &p_function);
	static bool is_function_constant(const StringName &p_function);

	static bool function_exists(const StringName &p_function);
	static void get_function_list(List<StringName> *r_functions);
	static MethodInfo get_function_info(const StringName &p_function);

	static void register_functions();
	static void unregister_functions();
};

#endif // GDSCRIPT_UTILITY_FUNCTIONS_H
