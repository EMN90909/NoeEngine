// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef WEBRTC_DATA_CHANNEL_EXTENSION_H
#define WEBRTC_DATA_CHANNEL_EXTENSION_H

#include "webrtc_data_channel.h"

#include "core/extension/ext_wrappers.gen.inc"
#include "core/object/gdvirtual.gen.inc"
#include "core/object/script_language.h"
#include "core/variant/native_ptr.h"

class WebRTCDataChannelExtension : public WebRTCDataChannel {
	GDCLASS(WebRTCDataChannelExtension, WebRTCDataChannel);

protected:
	static void _bind_methods();

public:
	EXBIND0R(Error, poll);
	EXBIND0(close);

	EXBIND1(set_write_mode, WriteMode);
	EXBIND0RC(WriteMode, get_write_mode);

	EXBIND0RC(bool, was_string_packet);

	EXBIND0RC(ChannelState, get_ready_state);
	EXBIND0RC(String, get_label);
	EXBIND0RC(bool, is_ordered);
	EXBIND0RC(int, get_id);
	EXBIND0RC(int, get_max_packet_life_time);
	EXBIND0RC(int, get_max_retransmits);
	EXBIND0RC(String, get_protocol);
	EXBIND0RC(bool, is_negotiated);
	EXBIND0RC(int, get_buffered_amount);

	/** Inherited from PacketPeer: **/
	EXBIND0RC(int, get_available_packet_count);
	EXBIND0RC(int, get_max_packet_size);
	virtual Error get_packet(const uint8_t **r_buffer, int &r_buffer_size) override; ///< buffer is GONE after next get_packet
	virtual Error put_packet(const uint8_t *p_buffer, int p_buffer_size) override;

	/** GDExtension **/
	GDVIRTUAL2R(Error, _get_packet, GDExtensionConstPtr<const uint8_t *>, GDExtensionPtr<int>);
	GDVIRTUAL2R(Error, _put_packet, GDExtensionConstPtr<const uint8_t>, int);

	WebRTCDataChannelExtension() {}
};

#endif // WEBRTC_DATA_CHANNEL_EXTENSION_H
