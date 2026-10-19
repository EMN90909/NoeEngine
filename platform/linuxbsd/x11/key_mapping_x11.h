// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef KEY_MAPPING_X11_H
#define KEY_MAPPING_X11_H

#include <X11/XF86keysym.h>
#include <X11/Xlib.h>
#define XK_MISCELLANY
#define XK_LATIN1
#define XK_XKB_KEYS
#include <X11/keysymdef.h>

#include "core/os/keyboard.h"
#include "core/templates/hash_map.h"

class KeyMappingX11 {
	struct HashMapHasherKeys {
		static _FORCE_INLINE_ uint32_t hash(const Key p_key) { return hash_fmix32(static_cast<uint32_t>(p_key)); }
		static _FORCE_INLINE_ uint32_t hash(const char32_t p_uchar) { return hash_fmix32(p_uchar); }
		static _FORCE_INLINE_ uint32_t hash(const unsigned p_key) { return hash_fmix32(p_key); }
		static _FORCE_INLINE_ uint32_t hash(const KeySym p_key) { return hash_fmix32(p_key); }
	};

	static inline HashMap<KeySym, Key, HashMapHasherKeys> xkeysym_map;
	static inline HashMap<unsigned int, Key, HashMapHasherKeys> scancode_map;
	static inline HashMap<Key, unsigned int, HashMapHasherKeys> scancode_map_inv;
	static inline HashMap<KeySym, char32_t, HashMapHasherKeys> xkeysym_unicode_map;

	KeyMappingX11() {}

public:
	static void initialize();

	static Key get_keycode(KeySym p_keysym);
	static unsigned int get_xlibcode(Key p_keysym);
	static Key get_scancode(unsigned int p_code);
	static char32_t get_unicode_from_keysym(KeySym p_keysym);
};

#endif // KEY_MAPPING_X11_H
