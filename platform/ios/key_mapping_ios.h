// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef KEY_MAPPING_IOS_H
#define KEY_MAPPING_IOS_H

#include "core/os/keyboard.h"

#import <UIKit/UIKit.h>

class KeyMappingIOS {
	KeyMappingIOS() {}

public:
	static void initialize();
	static Key remap_key(CFIndex p_keycode);
};

#endif // KEY_MAPPING_IOS_H
