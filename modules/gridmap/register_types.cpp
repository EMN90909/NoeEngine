// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef _3D_DISABLED

#include "register_types.h"

#include "core/object/class_db.h"
#include "grid_map.h"

#ifdef TOOLS_ENABLED
#include "editor/grid_map_editor_plugin.h"
#endif

void initialize_gridmap_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
		GDREGISTER_CLASS(GridMap);
	}
#ifdef TOOLS_ENABLED
	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR) {
		EditorPlugins::add_by_type<GridMapEditorPlugin>();
	}
#endif
}

void uninitialize_gridmap_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

#endif // _3D_DISABLED
