// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#import <OpenGLES/EAGLDrawable.h>
#import <QuartzCore/QuartzCore.h>

@protocol DisplayLayer <NSObject>

- (void)startRenderDisplayLayer;
- (void)stopRenderDisplayLayer;
- (void)initializeDisplayLayer;
- (void)layoutDisplayLayer;

@end

// An ugly workaround for iOS simulator
#if defined(TARGET_OS_SIMULATOR) && TARGET_OS_SIMULATOR
#if defined(__IPHONE_13_0)
API_AVAILABLE(ios(13.0))
@interface GodotMetalLayer : CAMetalLayer <DisplayLayer>
#else
@interface GodotMetalLayer : CALayer <DisplayLayer>
#endif
#else
@interface GodotMetalLayer : CAMetalLayer <DisplayLayer>
#endif
@end

API_DEPRECATED("OpenGLES is deprecated", ios(2.0, 12.0))
@interface GodotOpenGLLayer : CAEAGLLayer <DisplayLayer>

@end
