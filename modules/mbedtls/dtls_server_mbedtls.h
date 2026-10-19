// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef DTLS_SERVER_MBEDTLS_H
#define DTLS_SERVER_MBEDTLS_H

#include "core/io/dtls_server.h"
#include "tls_context_mbedtls.h"

class DTLSServerMbedTLS : public DTLSServer {
private:
	static DTLSServer *_create_func();
	Ref<TLSOptions> tls_options;
	Ref<CookieContextMbedTLS> cookies;

public:
	static void initialize();
	static void finalize();

	virtual Error setup(Ref<TLSOptions> p_options);
	virtual void stop();
	virtual Ref<PacketPeerDTLS> take_connection(Ref<PacketPeerUDP> p_peer);

	DTLSServerMbedTLS();
	~DTLSServerMbedTLS();
};

#endif // DTLS_SERVER_MBEDTLS_H
