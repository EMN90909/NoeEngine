// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef GODOT_WINDOW_H
#define GODOT_WINDOW_H

#include "servers/display_server.h"

#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>

@interface GodotWindow : NSWindow {
	DisplayServer::WindowID window_id;
	NSTimeInterval anim_duration;
}

- (void)setWindowID:(DisplayServer::WindowID)wid;
- (void)setAnimDuration:(NSTimeInterval)duration;

@end

#endif // GODOT_WINDOW_H
