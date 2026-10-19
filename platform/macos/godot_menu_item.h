// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef GODOT_MENU_ITEM_H
#define GODOT_MENU_ITEM_H

#include "servers/display_server.h"

#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>

enum GlobalMenuCheckType {
	CHECKABLE_TYPE_NONE,
	CHECKABLE_TYPE_CHECK_BOX,
	CHECKABLE_TYPE_RADIO_BUTTON,
};

@interface GodotMenuItem : NSObject {
@public
	Callable callback;
	Callable key_callback;
	Variant meta;
	GlobalMenuCheckType checkable_type;
	int max_states;
	int state;
	Ref<Image> img;
}

@end

#endif // GODOT_MENU_ITEM_H
