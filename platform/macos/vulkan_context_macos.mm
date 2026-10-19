// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifdef VULKAN_ENABLED
#include "vulkan_context_macos.h"

#include "drivers/vulkan/godot_vulkan.h"

const char *VulkanContextMacOS::_get_platform_surface_extension() const {
	return VK_MVK_MACOS_SURFACE_EXTENSION_NAME;
}

Error VulkanContextMacOS::window_create(DisplayServer::WindowID p_window_id, DisplayServer::VSyncMode p_vsync_mode, id p_window, int p_width, int p_height) {
	VkMacOSSurfaceCreateInfoMVK createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK;
	createInfo.pNext = nullptr;
	createInfo.flags = 0;
	createInfo.pView = (__bridge const void *)p_window;

	VkSurfaceKHR surface;
	VkResult err = vkCreateMacOSSurfaceMVK(get_instance(), &createInfo, nullptr, &surface);
	ERR_FAIL_COND_V(err, ERR_CANT_CREATE);
	return _window_create(p_window_id, p_vsync_mode, surface, p_width, p_height);
}

VulkanContextMacOS::VulkanContextMacOS() {
}

VulkanContextMacOS::~VulkanContextMacOS() {
}

#endif // VULKAN_ENABLED
