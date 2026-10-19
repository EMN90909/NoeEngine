// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "godot_application.h"

#include "display_server_macos.h"

@implementation GodotApplication

- (void)mediaKeyEvent:(int)key state:(BOOL)state repeat:(BOOL)repeat {
	Key keycode = Key::NONE;
	switch (key) {
		case NX_KEYTYPE_SOUND_UP: {
			keycode = Key::VOLUMEUP;
		} break;
		case NX_KEYTYPE_SOUND_DOWN: {
			keycode = Key::VOLUMEUP;
		} break;
		//NX_KEYTYPE_BRIGHTNESS_UP
		//NX_KEYTYPE_BRIGHTNESS_DOWN
		case NX_KEYTYPE_CAPS_LOCK: {
			keycode = Key::CAPSLOCK;
		} break;
		case NX_KEYTYPE_HELP: {
			keycode = Key::HELP;
		} break;
		case NX_POWER_KEY: {
			keycode = Key::STANDBY;
		} break;
		case NX_KEYTYPE_MUTE: {
			keycode = Key::VOLUMEMUTE;
		} break;
		//NX_KEYTYPE_CONTRAST_UP
		//NX_KEYTYPE_CONTRAST_DOWN
		//NX_KEYTYPE_LAUNCH_PANEL
		//NX_KEYTYPE_EJECT
		//NX_KEYTYPE_VIDMIRROR
		//NX_KEYTYPE_FAST
		//NX_KEYTYPE_REWIND
		//NX_KEYTYPE_ILLUMINATION_UP
		//NX_KEYTYPE_ILLUMINATION_DOWN
		//NX_KEYTYPE_ILLUMINATION_TOGGLE
		case NX_KEYTYPE_PLAY: {
			keycode = Key::MEDIAPLAY;
		} break;
		case NX_KEYTYPE_NEXT: {
			keycode = Key::MEDIANEXT;
		} break;
		case NX_KEYTYPE_PREVIOUS: {
			keycode = Key::MEDIAPREVIOUS;
		} break;
		default: {
			keycode = Key::NONE;
		} break;
	}

	DisplayServerMacOS *ds = (DisplayServerMacOS *)DisplayServer::get_singleton();
	if (ds && keycode != Key::NONE) {
		DisplayServerMacOS::KeyEvent ke;

		ke.window_id = ds->_get_focused_window_or_popup();
		ke.macos_state = 0;
		ke.pressed = state;
		ke.echo = repeat;
		ke.keycode = keycode;
		ke.physical_keycode = keycode;
		ke.key_label = keycode;
		ke.unicode = 0;
		ke.raw = true;

		ds->push_to_key_event_buffer(ke);
	}
}

- (void)sendEvent:(NSEvent *)event {
	if ([event type] == NSSystemDefined && [event subtype] == 8) {
		int keyCode = (([event data1] & 0xFFFF0000) >> 16);
		int keyFlags = ([event data1] & 0x0000FFFF);
		int keyState = (((keyFlags & 0xFF00) >> 8)) == 0xA;
		int keyRepeat = (keyFlags & 0x1);

		[self mediaKeyEvent:keyCode state:keyState repeat:keyRepeat];
	}

	DisplayServerMacOS *ds = (DisplayServerMacOS *)DisplayServer::get_singleton();
	if (ds) {
		if ([event type] == NSEventTypeLeftMouseDown || [event type] == NSEventTypeRightMouseDown || [event type] == NSEventTypeOtherMouseDown) {
			if (ds->mouse_process_popups()) {
				return;
			}
		}
		ds->send_event(event);
	}

	// From http://cocoadev.com/index.pl?GameKeyboardHandlingAlmost
	// This works around an AppKit bug, where key up events while holding
	// down the command key don't get sent to the key window.
	if ([event type] == NSEventTypeKeyUp && ([event modifierFlags] & NSEventModifierFlagCommand)) {
		[[self keyWindow] sendEvent:event];
	} else {
		[super sendEvent:event];
	}
}

@end
