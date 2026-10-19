// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "register_types.h"

#include "core/config/engine.h"
#include "servers/navigation_server_3d.h"

#include "godot_navigation_server.h"

#ifndef _3D_DISABLED
#include "navigation_mesh_generator.h"
#endif

#ifdef TOOLS_ENABLED
#include "editor/navigation_mesh_editor_plugin.h"
#endif

#ifndef _3D_DISABLED
NavigationMeshGenerator *_nav_mesh_generator = nullptr;
#endif

NavigationServer3D *new_server() {
	return memnew(GodotNavigationServer);
}

void initialize_navigation_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_SERVERS) {
		NavigationServer3DManager::set_default_server(new_server);

#ifndef _3D_DISABLED
		_nav_mesh_generator = memnew(NavigationMeshGenerator);
		GDREGISTER_CLASS(NavigationMeshGenerator);
		Engine::get_singleton()->add_singleton(Engine::Singleton("NavigationMeshGenerator", NavigationMeshGenerator::get_singleton()));
#endif
	}

#ifdef TOOLS_ENABLED
	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR) {
		EditorPlugins::add_by_type<NavigationMeshEditorPlugin>();
	}
#endif
}

void uninitialize_navigation_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SERVERS) {
		return;
	}

#ifndef _3D_DISABLED
	if (_nav_mesh_generator) {
		memdelete(_nav_mesh_generator);
	}
#endif
}
