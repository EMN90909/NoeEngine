// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#import <UIKit/UIKit.h>

class String;

@class GodotView;
@protocol DisplayLayer;
@protocol GodotViewRendererProtocol;

@protocol GodotViewDelegate

- (BOOL)godotViewFinishedSetup:(GodotView *)view;

@end

@interface GodotView : UIView

@property(assign, nonatomic) id<GodotViewRendererProtocol> renderer;
@property(assign, nonatomic) id<GodotViewDelegate> delegate;

@property(assign, readonly, nonatomic) BOOL isActive;

@property(assign, nonatomic) BOOL useCADisplayLink;
@property(strong, readonly, nonatomic) CALayer<DisplayLayer> *renderingLayer;
@property(assign, readonly, nonatomic) BOOL canRender;

@property(assign, nonatomic) NSTimeInterval renderingInterval;

- (CALayer<DisplayLayer> *)initializeRenderingForDriver:(NSString *)driverName;
- (void)stopRendering;
- (void)startRendering;

@end
