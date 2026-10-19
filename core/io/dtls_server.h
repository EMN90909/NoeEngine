// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef DTLS_SERVER_H
#define DTLS_SERVER_H

#include "core/io/net_socket.h"
#include "core/io/packet_peer_dtls.h"

class DTLSServer : public RefCounted {
	GDCLASS(DTLSServer, RefCounted);

protected:
	static DTLSServer *(*_create)();
	static void _bind_methods();

	static bool available;

public:
	static bool is_available();
	static DTLSServer *create();

	virtual Error setup(Ref<TLSOptions> p_options) = 0;
	virtual void stop() = 0;
	virtual Ref<PacketPeerDTLS> take_connection(Ref<PacketPeerUDP> p_peer) = 0;

	DTLSServer() {}
};

#endif // DTLS_SERVER_H
