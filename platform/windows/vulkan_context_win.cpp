// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#if defined(WINDOWS_ENABLED) && defined(VULKAN_ENABLED)

#include "vulkan_context_win.h"

#include "drivers/vulkan/godot_vulkan.h"

const char *VulkanContextWindows::_get_platform_surface_extension() const {
	return VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
}

Error VulkanContextWindows::window_create(DisplayServer::WindowID p_window_id, DisplayServer::VSyncMode p_vsync_mode, HWND p_window, HINSTANCE p_instance, int p_width, int p_height) {
	VkWin32SurfaceCreateInfoKHR createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	createInfo.pNext = nullptr;
	createInfo.flags = 0;
	createInfo.hinstance = p_instance;
	createInfo.hwnd = p_window;
	VkSurfaceKHR surface;
	VkResult err = vkCreateWin32SurfaceKHR(get_instance(), &createInfo, nullptr, &surface);
	ERR_FAIL_COND_V(err, ERR_CANT_CREATE);
	return _window_create(p_window_id, p_vsync_mode, surface, p_width, p_height);
}

VulkanContextWindows::VulkanContextWindows() {
	// Workaround for Vulkan not working on setups with AMD integrated graphics + NVIDIA dedicated GPU (GH-57708).
	// This prevents using AMD integrated graphics with Vulkan entirely, but it allows the engine to start
	// even on outdated/broken driver setups.
	OS::get_singleton()->set_environment("DISABLE_LAYER_AMD_SWITCHABLE_GRAPHICS_1", "1");
}

VulkanContextWindows::~VulkanContextWindows() {
}

#endif
