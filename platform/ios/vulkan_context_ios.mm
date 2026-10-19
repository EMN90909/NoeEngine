// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifdef VULKAN_ENABLED

#include "vulkan_context_ios.h"
#ifdef USE_VOLK
#include <volk.h>
#else
#include <vulkan/vulkan.h>
#endif

const char *VulkanContextIOS::_get_platform_surface_extension() const {
	return VK_MVK_IOS_SURFACE_EXTENSION_NAME;
}

Error VulkanContextIOS::window_create(DisplayServer::WindowID p_window_id, DisplayServer::VSyncMode p_vsync_mode, CALayer *p_metal_layer, int p_width, int p_height) {
	VkIOSSurfaceCreateInfoMVK createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_IOS_SURFACE_CREATE_INFO_MVK;
	createInfo.pNext = nullptr;
	createInfo.flags = 0;
	createInfo.pView = (__bridge const void *)p_metal_layer;

	VkSurfaceKHR surface;
	VkResult err =
			vkCreateIOSSurfaceMVK(get_instance(), &createInfo, nullptr, &surface);
	ERR_FAIL_COND_V(err, ERR_CANT_CREATE);

	return _window_create(p_window_id, p_vsync_mode, surface, p_width, p_height);
}

VulkanContextIOS::VulkanContextIOS() {}

VulkanContextIOS::~VulkanContextIOS() {}

#endif // VULKAN_ENABLED
