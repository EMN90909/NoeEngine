// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#import <UIKit/UIKit.h>

@class GodotView;
@class GodotNativeVideoView;
@class GodotKeyboardInputView;

@interface ViewController : UIViewController

@property(nonatomic, readonly, strong) GodotView *godotView;
@property(nonatomic, readonly, strong) GodotKeyboardInputView *keyboardView;

@end
