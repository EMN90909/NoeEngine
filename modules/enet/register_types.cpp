// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "register_types.h"
#include "core/error/error_macros.h"
#include "enet_connection.h"
#include "enet_multiplayer_peer.h"
#include "enet_packet_peer.h"

static bool enet_ok = false;

void initialize_enet_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	if (enet_initialize() != 0) {
		ERR_PRINT("ENet initialization failure");
	} else {
		enet_ok = true;
	}

	GDREGISTER_CLASS(ENetMultiplayerPeer);
	GDREGISTER_ABSTRACT_CLASS(ENetPacketPeer);
	GDREGISTER_CLASS(ENetConnection);
}

void uninitialize_enet_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	if (enet_ok) {
		enet_deinitialize();
	}
}
