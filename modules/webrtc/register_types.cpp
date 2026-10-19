// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "register_types.h"
#include "core/config/project_settings.h"
#include "webrtc_data_channel.h"
#include "webrtc_multiplayer_peer.h"
#include "webrtc_peer_connection.h"

#include "webrtc_data_channel_extension.h"
#include "webrtc_peer_connection_extension.h"

void initialize_webrtc_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

#define SET_HINT(NAME, _VAL_, _MAX_) \
	GLOBAL_DEF(PropertyInfo(Variant::INT, NAME, PROPERTY_HINT_RANGE, "2," #_MAX_ ",1,or_greater"), _VAL_);

	SET_HINT(WRTC_IN_BUF, 64, 4096);

	ClassDB::register_custom_instance_class<WebRTCPeerConnection>();
	GDREGISTER_CLASS(WebRTCPeerConnectionExtension);

	GDREGISTER_ABSTRACT_CLASS(WebRTCDataChannel);
	GDREGISTER_CLASS(WebRTCDataChannelExtension);

	GDREGISTER_CLASS(WebRTCMultiplayerPeer);

#undef SET_HINT
}

void uninitialize_webrtc_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}
