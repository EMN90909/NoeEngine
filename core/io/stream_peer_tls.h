// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef STREAM_PEER_TLS_H
#define STREAM_PEER_TLS_H

#include "core/crypto/crypto.h"
#include "core/io/stream_peer.h"

class StreamPeerTLS : public StreamPeer {
	GDCLASS(StreamPeerTLS, StreamPeer);

protected:
	static StreamPeerTLS *(*_create)();
	static void _bind_methods();

public:
	enum Status {
		STATUS_DISCONNECTED,
		STATUS_HANDSHAKING,
		STATUS_CONNECTED,
		STATUS_ERROR,
		STATUS_ERROR_HOSTNAME_MISMATCH
	};

	virtual void poll() = 0;
	virtual Error accept_stream(Ref<StreamPeer> p_base, Ref<TLSOptions> p_options) = 0;
	virtual Error connect_to_stream(Ref<StreamPeer> p_base, const String &p_common_name, Ref<TLSOptions> p_options) = 0;
	virtual Status get_status() const = 0;
	virtual Ref<StreamPeer> get_stream() const = 0;

	virtual void disconnect_from_stream() = 0;

	static StreamPeerTLS *create();

	static bool is_available();

	StreamPeerTLS() {}
};

VARIANT_ENUM_CAST(StreamPeerTLS::Status);

#endif // STREAM_PEER_TLS_H
