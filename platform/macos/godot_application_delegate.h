// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef GODOT_APPLICATION_DELEGATE_H
#define GODOT_APPLICATION_DELEGATE_H

#include "core/os/os.h"

#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>

@interface GodotApplicationDelegate : NSObject
- (void)forceUnbundledWindowActivationHackStep1;
- (void)forceUnbundledWindowActivationHackStep2;
- (void)forceUnbundledWindowActivationHackStep3;
- (void)handleAppleEvent:(NSAppleEventDescriptor *)event withReplyEvent:(NSAppleEventDescriptor *)replyEvent;
@end

#endif // GODOT_APPLICATION_DELEGATE_H
