// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef GODOT_CONTENT_VIEW_H
#define GODOT_CONTENT_VIEW_H

#include "servers/display_server.h"

#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>

#if defined(GLES3_ENABLED)
#import <AppKit/NSOpenGLView.h>
#define RootView NSOpenGLView
#else
#define RootView NSView
#endif

#import <QuartzCore/CAMetalLayer.h>

@interface GodotContentLayerDelegate : NSObject <CALayerDelegate> {
	DisplayServer::WindowID window_id;
}

- (void)setWindowID:(DisplayServer::WindowID)wid;

@end

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations" // OpenGL is deprecated in macOS 10.14

@interface GodotContentView : RootView <NSTextInputClient> {
	DisplayServer::WindowID window_id;
	NSTrackingArea *tracking_area;
	NSMutableAttributedString *marked_text;
	bool ime_input_event_in_progress;
	bool mouse_down_control;
	bool ignore_momentum_scroll;
	bool last_pen_inverted;
	bool ime_suppress_next_keyup;
	id layer_delegate;
}

- (void)processScrollEvent:(NSEvent *)event button:(MouseButton)button factor:(double)factor;
- (void)processPanEvent:(NSEvent *)event dx:(double)dx dy:(double)dy;
- (void)processMouseEvent:(NSEvent *)event index:(MouseButton)index pressed:(bool)pressed;
- (void)setWindowID:(DisplayServer::WindowID)wid;
- (void)updateLayerDelegate;
- (void)cancelComposition;

@end

#pragma clang diagnostic pop

#endif // GODOT_CONTENT_VIEW_H
