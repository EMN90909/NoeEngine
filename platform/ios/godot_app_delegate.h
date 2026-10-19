// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#import <UIKit/UIKit.h>

typedef NSObject<UIApplicationDelegate> ApplicationDelegateService;

@interface GodotApplicalitionDelegate : NSObject <UIApplicationDelegate>

@property(class, readonly, strong) NSArray<ApplicationDelegateService *> *services;

+ (void)addService:(ApplicationDelegateService *)service;

@end
