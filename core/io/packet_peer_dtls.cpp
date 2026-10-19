// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "packet_peer_dtls.h"
#include "core/config/project_settings.h"
#include "core/io/file_access.h"

PacketPeerDTLS *(*PacketPeerDTLS::_create)() = nullptr;
bool PacketPeerDTLS::available = false;

PacketPeerDTLS *PacketPeerDTLS::create() {
	if (_create) {
		return _create();
	}
	return nullptr;
}

bool PacketPeerDTLS::is_available() {
	return available;
}

void PacketPeerDTLS::_bind_methods() {
	ClassDB::bind_method(D_METHOD("poll"), &PacketPeerDTLS::poll);
	ClassDB::bind_method(D_METHOD("connect_to_peer", "packet_peer", "hostname", "client_options"), &PacketPeerDTLS::connect_to_peer, DEFVAL(Ref<TLSOptions>()));
	ClassDB::bind_method(D_METHOD("get_status"), &PacketPeerDTLS::get_status);
	ClassDB::bind_method(D_METHOD("disconnect_from_peer"), &PacketPeerDTLS::disconnect_from_peer);

	BIND_ENUM_CONSTANT(STATUS_DISCONNECTED);
	BIND_ENUM_CONSTANT(STATUS_HANDSHAKING);
	BIND_ENUM_CONSTANT(STATUS_CONNECTED);
	BIND_ENUM_CONSTANT(STATUS_ERROR);
	BIND_ENUM_CONSTANT(STATUS_ERROR_HOSTNAME_MISMATCH);
}
