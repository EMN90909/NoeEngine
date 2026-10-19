// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef CORE_GLOBALS_H
#define CORE_GLOBALS_H

// Home for state needed from global functions
// that cannot be stored in Engine or OS due to e.g. circular includes

class CoreGlobals {
public:
	static bool leak_reporting_enabled;
	static bool print_line_enabled;
	static bool print_error_enabled;
};

#endif // CORE_GLOBALS_H
