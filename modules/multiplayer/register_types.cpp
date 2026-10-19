// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "register_types.h"

#include "multiplayer_spawner.h"
#include "multiplayer_synchronizer.h"
#include "scene_multiplayer.h"
#include "scene_replication_interface.h"
#include "scene_rpc_interface.h"

#include "multiplayer_debugger.h"

#ifdef TOOLS_ENABLED
#include "editor/multiplayer_editor_plugin.h"
#endif

void initialize_multiplayer_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
		GDREGISTER_CLASS(SceneReplicationConfig);
		GDREGISTER_CLASS(MultiplayerSpawner);
		GDREGISTER_CLASS(MultiplayerSynchronizer);
		GDREGISTER_CLASS(OfflineMultiplayerPeer);
		GDREGISTER_CLASS(SceneMultiplayer);
		MultiplayerAPI::set_default_interface("SceneMultiplayer");
		MultiplayerDebugger::initialize();
	}
#ifdef TOOLS_ENABLED
	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR) {
		EditorPlugins::add_by_type<MultiplayerEditorPlugin>();
	}
#endif
}

void uninitialize_multiplayer_module(ModuleInitializationLevel p_level) {
	MultiplayerDebugger::deinitialize();
}
