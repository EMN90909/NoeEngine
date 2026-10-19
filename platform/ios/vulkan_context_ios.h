// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef VULKAN_CONTEXT_IOS_H
#define VULKAN_CONTEXT_IOS_H

#ifdef VULKAN_ENABLED

#include "drivers/vulkan/vulkan_context.h"

#import <UIKit/UIKit.h>

class VulkanContextIOS : public VulkanContext {
	virtual const char *_get_platform_surface_extension() const;

public:
	Error window_create(DisplayServer::WindowID p_window_id, DisplayServer::VSyncMode p_vsync_mode, CALayer *p_metal_layer, int p_width, int p_height);

	VulkanContextIOS();
	~VulkanContextIOS();
};

#endif // VULKAN_ENABLED

#endif // VULKAN_CONTEXT_IOS_H
