// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "stream_peer_tls.h"

#include "core/config/engine.h"

StreamPeerTLS *(*StreamPeerTLS::_create)() = nullptr;

StreamPeerTLS *StreamPeerTLS::create() {
	if (_create) {
		return _create();
	}
	return nullptr;
}

bool StreamPeerTLS::is_available() {
	return _create != nullptr;
}

void StreamPeerTLS::_bind_methods() {
	ClassDB::bind_method(D_METHOD("poll"), &StreamPeerTLS::poll);
	ClassDB::bind_method(D_METHOD("accept_stream", "stream", "server_options"), &StreamPeerTLS::accept_stream);
	ClassDB::bind_method(D_METHOD("connect_to_stream", "stream", "common_name", "client_options"), &StreamPeerTLS::connect_to_stream, DEFVAL(Ref<TLSOptions>()));
	ClassDB::bind_method(D_METHOD("get_status"), &StreamPeerTLS::get_status);
	ClassDB::bind_method(D_METHOD("get_stream"), &StreamPeerTLS::get_stream);
	ClassDB::bind_method(D_METHOD("disconnect_from_stream"), &StreamPeerTLS::disconnect_from_stream);

	BIND_ENUM_CONSTANT(STATUS_DISCONNECTED);
	BIND_ENUM_CONSTANT(STATUS_HANDSHAKING);
	BIND_ENUM_CONSTANT(STATUS_CONNECTED);
	BIND_ENUM_CONSTANT(STATUS_ERROR);
	BIND_ENUM_CONSTANT(STATUS_ERROR_HOSTNAME_MISMATCH);
}
