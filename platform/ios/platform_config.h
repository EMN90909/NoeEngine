// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include <alloca.h>

#define OPENGL_INCLUDE_H <ES3/gl.h>

#define PTHREAD_RENAME_SELF

#define _weakify(var) __weak typeof(var) GDWeak_##var = var;
#define _strongify(var)                                      \
	_Pragma("clang diagnostic push")                         \
			_Pragma("clang diagnostic ignored \"-Wshadow\"") \
					__strong typeof(var) var = GDWeak_##var; \
	_Pragma("clang diagnostic pop")
