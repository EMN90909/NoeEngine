// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef VULKAN_CONTEXT_MACOS_H
#define VULKAN_CONTEXT_MACOS_H

#ifdef VULKAN_ENABLED

#include "drivers/vulkan/vulkan_context.h"
#import <AppKit/AppKit.h>

class VulkanContextMacOS : public VulkanContext {
	virtual const char *_get_platform_surface_extension() const;

public:
	Error window_create(DisplayServer::WindowID p_window_id, DisplayServer::VSyncMode p_vsync_mode, id p_window, int p_width, int p_height);

	VulkanContextMacOS();
	~VulkanContextMacOS();
};

#endif // VULKAN_ENABLED

#endif // VULKAN_CONTEXT_MACOS_H
