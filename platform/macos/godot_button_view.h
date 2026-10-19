// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef GODOT_BUTTON_VIEW_H
#define GODOT_BUTTON_VIEW_H

#include "servers/display_server.h"

#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>

@interface GodotButtonView : NSView {
	NSTrackingArea *tracking_area;
	NSPoint offset;
	CGFloat spacing;
	bool mouse_in_group;
	bool rtl;
	NSButton *close_button;
	NSButton *miniaturize_button;
	NSButton *zoom_button;
}

- (void)initButtons:(CGFloat)button_spacing offset:(NSPoint)button_offset rtl:(bool)is_rtl;
- (void)displayButtons;
- (void)setOffset:(NSPoint)button_offset;
- (NSPoint)getOffset;

@end

#endif // GODOT_BUTTON_VIEW_H
