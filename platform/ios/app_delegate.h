// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#import <UIKit/UIKit.h>

@class ViewController;

// FIXME: Add support for both OpenGL and Vulkan when OpenGL is implemented again,
// so it can't be done with compilation time branching.
//#if defined(GLES3_ENABLED)
//@interface AppDelegate : NSObject <UIApplicationDelegate, GLViewDelegate> {
//#endif
//#if defined(VULKAN_ENABLED)
@interface AppDelegate : NSObject <UIApplicationDelegate>
//#endif

@property(strong, nonatomic) UIWindow *window;
@property(strong, class, readonly, nonatomic) ViewController *viewController;

@end
