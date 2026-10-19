// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "register_types.h"
#include "scene/resources/surface_tool.h"
#include "thirdparty/meshoptimizer/meshoptimizer.h"

void initialize_meshoptimizer_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	SurfaceTool::optimize_vertex_cache_func = meshopt_optimizeVertexCache;
	SurfaceTool::simplify_func = meshopt_simplify;
	SurfaceTool::simplify_with_attrib_func = meshopt_simplifyWithAttributes;
	SurfaceTool::simplify_scale_func = meshopt_simplifyScale;
	SurfaceTool::simplify_sloppy_func = meshopt_simplifySloppy;
	SurfaceTool::generate_remap_func = meshopt_generateVertexRemap;
	SurfaceTool::remap_vertex_func = meshopt_remapVertexBuffer;
	SurfaceTool::remap_index_func = meshopt_remapIndexBuffer;
}

void uninitialize_meshoptimizer_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	SurfaceTool::optimize_vertex_cache_func = nullptr;
	SurfaceTool::simplify_func = nullptr;
	SurfaceTool::simplify_scale_func = nullptr;
	SurfaceTool::simplify_sloppy_func = nullptr;
	SurfaceTool::generate_remap_func = nullptr;
	SurfaceTool::remap_vertex_func = nullptr;
	SurfaceTool::remap_index_func = nullptr;
}
