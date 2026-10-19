// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "register_types.h"

#include "core/config/project_settings.h"
#include "lightmapper_rd.h"
#include "scene/3d/lightmapper.h"

#ifndef _3D_DISABLED
static Lightmapper *create_lightmapper_rd() {
	return memnew(LightmapperRD);
}
#endif

void initialize_lightmapper_rd_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	GLOBAL_DEF("rendering/lightmapping/bake_quality/low_quality_ray_count", 16);
	GLOBAL_DEF("rendering/lightmapping/bake_quality/medium_quality_ray_count", 64);
	GLOBAL_DEF("rendering/lightmapping/bake_quality/high_quality_ray_count", 256);
	GLOBAL_DEF("rendering/lightmapping/bake_quality/ultra_quality_ray_count", 1024);
	GLOBAL_DEF("rendering/lightmapping/bake_performance/max_rays_per_pass", 32);
	GLOBAL_DEF("rendering/lightmapping/bake_performance/region_size", 512);

	GLOBAL_DEF("rendering/lightmapping/bake_quality/low_quality_probe_ray_count", 64);
	GLOBAL_DEF("rendering/lightmapping/bake_quality/medium_quality_probe_ray_count", 256);
	GLOBAL_DEF("rendering/lightmapping/bake_quality/high_quality_probe_ray_count", 512);
	GLOBAL_DEF("rendering/lightmapping/bake_quality/ultra_quality_probe_ray_count", 2048);
	GLOBAL_DEF("rendering/lightmapping/bake_performance/max_rays_per_probe_pass", 64);
	GLOBAL_DEF("rendering/lightmapping/primitive_meshes/texel_size", 0.2);
#ifndef _3D_DISABLED
	GDREGISTER_CLASS(LightmapperRD);
	Lightmapper::create_gpu = create_lightmapper_rd;
#endif
}

void uninitialize_lightmapper_rd_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}
