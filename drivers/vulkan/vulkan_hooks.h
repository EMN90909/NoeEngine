// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef VULKAN_HOOKS_H
#define VULKAN_HOOKS_H

#include "drivers/vulkan/godot_vulkan.h"

class VulkanHooks {
public:
	virtual bool create_vulkan_instance(const VkInstanceCreateInfo *p_vulkan_create_info, VkInstance *r_instance) { return false; };
	virtual bool get_physical_device(VkPhysicalDevice *r_device) { return false; };
	virtual bool create_vulkan_device(const VkDeviceCreateInfo *p_device_create_info, VkDevice *r_device) { return false; };
	virtual ~VulkanHooks(){};
};

#endif // VULKAN_HOOKS_H
