// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#import "godot_app_delegate.h"

#import <UIKit/UIKit.h>
#include <stdio.h>

int gargc;
char **gargv;

int main(int argc, char *argv[]) {
#if defined(VULKAN_ENABLED)
	//MoltenVK - enable full component swizzling support
	setenv("MVK_CONFIG_FULL_IMAGE_VIEW_SWIZZLE", "1", 1);
#endif

	printf("*********** main.m\n");
	gargc = argc;
	gargv = argv;

	printf("running app main\n");
	@autoreleasepool {
		NSString *className = NSStringFromClass([GodotApplicalitionDelegate class]);
		UIApplicationMain(argc, argv, nil, className);
	}
	printf("main done\n");
	return 0;
}
