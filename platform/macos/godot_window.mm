// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "godot_window.h"

#include "display_server_macos.h"

@implementation GodotWindow

- (id)init {
	self = [super init];
	window_id = DisplayServer::INVALID_WINDOW_ID;
	anim_duration = -1.0f;
	return self;
}

- (void)setAnimDuration:(NSTimeInterval)duration {
	anim_duration = duration;
}

- (NSTimeInterval)animationResizeTime:(NSRect)newFrame {
	if (anim_duration > 0) {
		return anim_duration;
	} else {
		return [super animationResizeTime:newFrame];
	}
}

- (void)setWindowID:(DisplayServerMacOS::WindowID)wid {
	window_id = wid;
}

- (BOOL)canBecomeKeyWindow {
	// Required for NSWindowStyleMaskBorderless windows.
	DisplayServerMacOS *ds = (DisplayServerMacOS *)DisplayServer::get_singleton();
	if (!ds || !ds->has_window(window_id)) {
		return YES;
	}

	DisplayServerMacOS::WindowData &wd = ds->get_window(window_id);
	return !wd.no_focus && !wd.is_popup;
}

- (BOOL)canBecomeMainWindow {
	// Required for NSWindowStyleMaskBorderless windows.
	DisplayServerMacOS *ds = (DisplayServerMacOS *)DisplayServer::get_singleton();
	if (!ds || !ds->has_window(window_id)) {
		return YES;
	}

	DisplayServerMacOS::WindowData &wd = ds->get_window(window_id);
	return !wd.no_focus && !wd.is_popup;
}

@end
