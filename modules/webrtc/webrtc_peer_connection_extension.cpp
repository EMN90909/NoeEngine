// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "webrtc_peer_connection_extension.h"

void WebRTCPeerConnectionExtension::_bind_methods() {
	GDVIRTUAL_BIND(_get_connection_state);
	GDVIRTUAL_BIND(_get_gathering_state);
	GDVIRTUAL_BIND(_get_signaling_state);
	GDVIRTUAL_BIND(_initialize, "p_config");
	GDVIRTUAL_BIND(_create_data_channel, "p_label", "p_config");
	GDVIRTUAL_BIND(_create_offer);
	GDVIRTUAL_BIND(_set_remote_description, "p_type", "p_sdp");
	GDVIRTUAL_BIND(_set_local_description, "p_type", "p_sdp");
	GDVIRTUAL_BIND(_add_ice_candidate, "p_sdp_mid_name", "p_sdp_mline_index", "p_sdp_name");
	GDVIRTUAL_BIND(_poll);
	GDVIRTUAL_BIND(_close);
}

Ref<WebRTCDataChannel> WebRTCPeerConnectionExtension::create_data_channel(String p_label, Dictionary p_options) {
	Object *ret = nullptr;
	if (GDVIRTUAL_CALL(_create_data_channel, p_label, p_options, ret)) {
		WebRTCDataChannel *ch = Object::cast_to<WebRTCDataChannel>(ret);
		ERR_FAIL_COND_V_MSG(ret && !ch, nullptr, "Returned object must be an instance of WebRTCDataChannel.");
		return ch;
	}
	WARN_PRINT_ONCE("WebRTCPeerConnectionExtension::_create_data_channel is unimplemented!");
	return nullptr;
}
