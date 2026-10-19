// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef SCRIPT_ITERATOR_H
#define SCRIPT_ITERATOR_H

#ifdef GDEXTENSION

// Headers for building as GDExtension plug-in.
#include <godot_cpp/godot.hpp>
#include <godot_cpp/templates/vector.hpp>
#include <godot_cpp/variant/string.hpp>

using namespace godot;

#else

// Headers for building as built-in module.
#include "core/string/ustring.h"
#include "core/templates/vector.h"

#endif

#include <unicode/uchar.h>
#include <unicode/uloc.h>
#include <unicode/uscript.h>
#include <unicode/ustring.h>
#include <unicode/utypes.h>

#include <hb-icu.h>
#include <hb.h>

class ScriptIterator {
	static const int PAREN_STACK_DEPTH = 128;

public:
	struct ScriptRange {
		int start = 0;
		int end = 0;
		hb_script_t script = HB_SCRIPT_COMMON;
	};
	Vector<ScriptRange> script_ranges;

private:
	static bool same_script(int32_t p_script_one, int32_t p_script_two);

public:
	ScriptIterator(const String &p_string, int p_start, int p_length);
};

#endif // SCRIPT_ITERATOR_H
