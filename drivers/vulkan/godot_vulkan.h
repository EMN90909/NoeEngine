// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef GODOT_VULKAN_H
#define GODOT_VULKAN_H

#ifdef USE_VOLK
#include <volk.h>
#else
#include <stdint.h>
#define VK_NO_STDINT_H
#include <vulkan/vulkan.h>
#endif

#endif // GODOT_VULKAN_H
