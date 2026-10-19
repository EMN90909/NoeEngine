// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef VULKAN_CONTEXT_ANDROID_H
#define VULKAN_CONTEXT_ANDROID_H

#ifdef VULKAN_ENABLED

#include "drivers/vulkan/vulkan_context.h"

struct ANativeWindow;

class VulkanContextAndroid : public VulkanContext {
	virtual const char *_get_platform_surface_extension() const override;

public:
	Error window_create(ANativeWindow *p_window, DisplayServer::VSyncMode p_vsync_mode, int p_width, int p_height);

	VulkanContextAndroid() = default;
	~VulkanContextAndroid() override = default;

protected:
	bool _use_validation_layers() override;
};

#endif // VULKAN_ENABLED

#endif // VULKAN_CONTEXT_ANDROID_H
