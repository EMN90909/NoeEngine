// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef PRINT_STRING_H
#define PRINT_STRING_H

#include "core/variant/variant.h"

extern void (*_print_func)(String);

typedef void (*PrintHandlerFunc)(void *, const String &p_string, bool p_error, bool p_rich);

struct PrintHandlerList {
	PrintHandlerFunc printfunc = nullptr;
	void *userdata = nullptr;

	PrintHandlerList *next = nullptr;

	PrintHandlerList() {}
};

String stringify_variants(Variant p_var);

template <typename... Args>
String stringify_variants(Variant p_var, Args... p_args) {
	return p_var.operator String() + " " + stringify_variants(p_args...);
}

void add_print_handler(PrintHandlerList *p_handler);
void remove_print_handler(const PrintHandlerList *p_handler);

extern void __print_line(String p_string);
extern void __print_line_rich(String p_string);
extern void print_error(String p_string);
extern void print_verbose(String p_string);

inline void print_line(Variant v) {
	__print_line(stringify_variants(v));
}

inline void print_line_rich(Variant v) {
	__print_line_rich(stringify_variants(v));
}

template <typename... Args>
void print_line(Variant p_var, Args... p_args) {
	__print_line(stringify_variants(p_var, p_args...));
}

template <typename... Args>
void print_line_rich(Variant p_var, Args... p_args) {
	__print_line_rich(stringify_variants(p_var, p_args...));
}

#endif // PRINT_STRING_H
