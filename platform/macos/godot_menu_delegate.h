// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef GODOT_MENU_DELEGATE_H
#define GODOT_MENU_DELEGATE_H

#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>

@interface GodotMenuDelegate : NSObject <NSMenuDelegate> {
}

- (void)doNothing:(id)sender;

@end

#endif // GODOT_MENU_DELEGATE_H
