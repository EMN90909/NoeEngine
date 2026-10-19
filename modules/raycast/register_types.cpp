// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "register_types.h"

#include "lightmap_raycaster_embree.h"
#include "raycast_occlusion_cull.h"
#include "static_raycaster_embree.h"

RaycastOcclusionCull *raycast_occlusion_cull = nullptr;

void initialize_raycast_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

#ifdef TOOLS_ENABLED
	LightmapRaycasterEmbree::make_default_raycaster();
	StaticRaycasterEmbree::make_default_raycaster();
#endif
	raycast_occlusion_cull = memnew(RaycastOcclusionCull);
}

void uninitialize_raycast_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	if (raycast_occlusion_cull) {
		memdelete(raycast_occlusion_cull);
	}
#ifdef TOOLS_ENABLED
	StaticRaycasterEmbree::free();
#endif
}
