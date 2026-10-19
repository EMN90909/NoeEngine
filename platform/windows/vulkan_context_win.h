// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef VULKAN_CONTEXT_WIN_H
#define VULKAN_CONTEXT_WIN_H

#ifdef VULKAN_ENABLED

#include "drivers/vulkan/vulkan_context.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class VulkanContextWindows : public VulkanContext {
	virtual const char *_get_platform_surface_extension() const;

public:
	Error window_create(DisplayServer::WindowID p_window_id, DisplayServer::VSyncMode p_vsync_mode, HWND p_window, HINSTANCE p_instance, int p_width, int p_height);

	VulkanContextWindows();
	~VulkanContextWindows();
};

#endif // VULKAN_ENABLED

#endif // VULKAN_CONTEXT_WIN_H
