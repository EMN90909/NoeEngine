// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef WEBRTC_PEER_CONNECTION_EXTENSION_H
#define WEBRTC_PEER_CONNECTION_EXTENSION_H

#include "webrtc_peer_connection.h"

#include "core/extension/ext_wrappers.gen.inc"
#include "core/object/gdvirtual.gen.inc"
#include "core/object/script_language.h"
#include "core/variant/native_ptr.h"

class WebRTCPeerConnectionExtension : public WebRTCPeerConnection {
	GDCLASS(WebRTCPeerConnectionExtension, WebRTCPeerConnection);

protected:
	static void _bind_methods();

public:
	// FIXME Can't be directly exposed due to issues in exchanging Ref(s) between godot and extensions.
	// See godot-cpp GH-652 .
	virtual Ref<WebRTCDataChannel> create_data_channel(String p_label, Dictionary p_options = Dictionary()) override;
	GDVIRTUAL2R(Object *, _create_data_channel, String, Dictionary);
	// EXBIND2R(Ref<WebRTCDataChannel>, create_data_channel, String, Dictionary);

	/** GDExtension **/
	EXBIND0RC(ConnectionState, get_connection_state);
	EXBIND0RC(GatheringState, get_gathering_state);
	EXBIND0RC(SignalingState, get_signaling_state);
	EXBIND1R(Error, initialize, Dictionary);
	EXBIND0R(Error, create_offer);
	EXBIND2R(Error, set_remote_description, String, String);
	EXBIND2R(Error, set_local_description, String, String);
	EXBIND3R(Error, add_ice_candidate, String, int, String);
	EXBIND0R(Error, poll);
	EXBIND0(close);

	WebRTCPeerConnectionExtension() {}
};

#endif // WEBRTC_PEER_CONNECTION_EXTENSION_H
