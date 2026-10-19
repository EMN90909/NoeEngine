// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifdef VULKAN_ENABLED

#include "vulkan_context_android.h"

#include "drivers/vulkan/godot_vulkan.h"

const char *VulkanContextAndroid::_get_platform_surface_extension() const {
	return VK_KHR_ANDROID_SURFACE_EXTENSION_NAME;
}

Error VulkanContextAndroid::window_create(ANativeWindow *p_window, DisplayServer::VSyncMode p_vsync_mode, int p_width, int p_height) {
	VkAndroidSurfaceCreateInfoKHR createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
	createInfo.pNext = nullptr;
	createInfo.flags = 0;
	createInfo.window = p_window;

	VkSurfaceKHR surface;
	VkResult err = vkCreateAndroidSurfaceKHR(get_instance(), &createInfo, nullptr, &surface);
	if (err != VK_SUCCESS) {
		ERR_FAIL_V_MSG(ERR_CANT_CREATE, "vkCreateAndroidSurfaceKHR failed with error " + itos(err));
	}

	return _window_create(DisplayServer::MAIN_WINDOW_ID, p_vsync_mode, surface, p_width, p_height);
}

bool VulkanContextAndroid::_use_validation_layers() {
	uint32_t count = 0;
	_get_preferred_validation_layers(&count, nullptr);

	// On Android, we use validation layers automatically if they were explicitly linked with the app.
	return count > 0;
}

#endif // VULKAN_ENABLED
