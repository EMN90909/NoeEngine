// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef KEY_MAPPING_MACOS_H
#define KEY_MAPPING_MACOS_H

#include "core/os/keyboard.h"

class KeyMappingMacOS {
	KeyMappingMacOS() {}

	static bool is_numpad_key(unsigned int p_key);

public:
	static void initialize();

	// Mappings input.
	static Key translate_key(unsigned int p_key);
	static unsigned int unmap_key(Key p_key);
	static Key remap_key(unsigned int p_key, unsigned int p_state, bool p_unicode);

	// Mapping for menu shortcuts.
	static String keycode_get_native_string(Key p_keycode);
	static unsigned int keycode_get_native_mask(Key p_keycode);
};

#endif // KEY_MAPPING_MACOS_H
