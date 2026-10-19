// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "dtls_server_mbedtls.h"
#include "packet_peer_mbed_dtls.h"

Error DTLSServerMbedTLS::setup(Ref<TLSOptions> p_options) {
	ERR_FAIL_COND_V(p_options.is_null() || !p_options->is_server(), ERR_INVALID_PARAMETER);
	ERR_FAIL_COND_V(cookies->setup() != OK, ERR_ALREADY_IN_USE);
	tls_options = p_options;
	return OK;
}

void DTLSServerMbedTLS::stop() {
	cookies->clear();
}

Ref<PacketPeerDTLS> DTLSServerMbedTLS::take_connection(Ref<PacketPeerUDP> p_udp_peer) {
	Ref<PacketPeerMbedDTLS> out;

	ERR_FAIL_COND_V(tls_options.is_null(), out);
	ERR_FAIL_COND_V(!p_udp_peer.is_valid(), out);

	out.instantiate();
	out->accept_peer(p_udp_peer, tls_options, cookies);
	return out;
}

DTLSServer *DTLSServerMbedTLS::_create_func() {
	return memnew(DTLSServerMbedTLS);
}

void DTLSServerMbedTLS::initialize() {
	_create = _create_func;
	available = true;
}

void DTLSServerMbedTLS::finalize() {
	_create = nullptr;
	available = false;
}

DTLSServerMbedTLS::DTLSServerMbedTLS() {
	cookies.instantiate();
}

DTLSServerMbedTLS::~DTLSServerMbedTLS() {
	stop();
}
