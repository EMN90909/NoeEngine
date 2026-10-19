// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "godot_menu_delegate.h"

#include "display_server_macos.h"
#include "godot_menu_item.h"
#include "key_mapping_macos.h"

@implementation GodotMenuDelegate

- (void)doNothing:(id)sender {
}

- (BOOL)menuHasKeyEquivalent:(NSMenu *)menu forEvent:(NSEvent *)event target:(id *)target action:(SEL *)action {
	NSString *ev_key = [[event charactersIgnoringModifiers] lowercaseString];
	NSUInteger ev_modifiers = [event modifierFlags] & NSEventModifierFlagDeviceIndependentFlagsMask;
	for (int i = 0; i < [menu numberOfItems]; i++) {
		const NSMenuItem *menu_item = [menu itemAtIndex:i];
		if ([menu_item isEnabled] && [[menu_item keyEquivalent] compare:ev_key] == NSOrderedSame) {
			NSUInteger item_modifiers = [menu_item keyEquivalentModifierMask];

			if (ev_modifiers == item_modifiers) {
				GodotMenuItem *value = [menu_item representedObject];
				if (value->key_callback != Callable()) {
					// If custom callback is set, use it.
					Variant tag = value->meta;
					Variant *tagp = &tag;
					Variant ret;
					Callable::CallError ce;
					value->key_callback.callp((const Variant **)&tagp, 1, ret, ce);
				} else {
					// Otherwise redirect event to the engine.
					if (DisplayServer::get_singleton()) {
						[[[NSApplication sharedApplication] keyWindow] sendEvent:event];
					}
				}

				// Suppress default menu action.
				*target = self;
				*action = @selector(doNothing:);
				return YES;
			}
		}
	}
	return NO;
}

@end
