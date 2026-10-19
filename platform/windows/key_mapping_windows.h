// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef KEY_MAPPING_WINDOWS_H
#define KEY_MAPPING_WINDOWS_H

#include "core/os/keyboard.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winuser.h>

class KeyMappingWindows {
	KeyMappingWindows() {}

public:
	static void initialize();

	static Key get_keysym(unsigned int p_code);
	static unsigned int get_scancode(Key p_keycode);
	static Key get_scansym(unsigned int p_code, bool p_extended);
	static bool is_extended_key(unsigned int p_code);
};

#endif // KEY_MAPPING_WINDOWS_H
