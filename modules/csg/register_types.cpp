// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "register_types.h"

#ifndef _3D_DISABLED

#include "csg_shape.h"

#ifdef TOOLS_ENABLED
#include "editor/csg_gizmos.h"
#endif

void initialize_csg_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
		GDREGISTER_ABSTRACT_CLASS(CSGShape3D);
		GDREGISTER_ABSTRACT_CLASS(CSGPrimitive3D);
		GDREGISTER_CLASS(CSGMesh3D);
		GDREGISTER_CLASS(CSGSphere3D);
		GDREGISTER_CLASS(CSGBox3D);
		GDREGISTER_CLASS(CSGCylinder3D);
		GDREGISTER_CLASS(CSGTorus3D);
		GDREGISTER_CLASS(CSGPolygon3D);
		GDREGISTER_CLASS(CSGCombiner3D);
	}
#ifdef TOOLS_ENABLED
	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR) {
		EditorPlugins::add_by_type<EditorPluginCSG>();
	}
#endif
}

void uninitialize_csg_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

#endif // _3D_DISABLED
