// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "webrtc_data_channel_extension.h"

void WebRTCDataChannelExtension::_bind_methods() {
	ADD_PROPERTY_DEFAULT("write_mode", WRITE_MODE_BINARY);

	GDVIRTUAL_BIND(_get_packet, "r_buffer", "r_buffer_size");
	GDVIRTUAL_BIND(_put_packet, "p_buffer", "p_buffer_size");
	GDVIRTUAL_BIND(_get_available_packet_count);
	GDVIRTUAL_BIND(_get_max_packet_size);

	GDVIRTUAL_BIND(_poll);
	GDVIRTUAL_BIND(_close);

	GDVIRTUAL_BIND(_set_write_mode, "p_write_mode");
	GDVIRTUAL_BIND(_get_write_mode);

	GDVIRTUAL_BIND(_was_string_packet);
	GDVIRTUAL_BIND(_get_ready_state);
	GDVIRTUAL_BIND(_get_label);
	GDVIRTUAL_BIND(_is_ordered);
	GDVIRTUAL_BIND(_get_id);
	GDVIRTUAL_BIND(_get_max_packet_life_time);
	GDVIRTUAL_BIND(_get_max_retransmits);
	GDVIRTUAL_BIND(_get_protocol);
	GDVIRTUAL_BIND(_is_negotiated);
	GDVIRTUAL_BIND(_get_buffered_amount);
}

Error WebRTCDataChannelExtension::get_packet(const uint8_t **r_buffer, int &r_buffer_size) {
	Error err;
	if (GDVIRTUAL_CALL(_get_packet, r_buffer, &r_buffer_size, err)) {
		return err;
	}
	WARN_PRINT_ONCE("WebRTCDataChannelExtension::_get_packet_native is unimplemented!");
	return FAILED;
}

Error WebRTCDataChannelExtension::put_packet(const uint8_t *p_buffer, int p_buffer_size) {
	Error err;
	if (GDVIRTUAL_CALL(_put_packet, p_buffer, p_buffer_size, err)) {
		return err;
	}
	WARN_PRINT_ONCE("WebRTCDataChannelExtension::_put_packet_native is unimplemented!");
	return FAILED;
}
