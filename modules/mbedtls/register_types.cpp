// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "register_types.h"

#include "crypto_mbedtls.h"
#include "dtls_server_mbedtls.h"
#include "packet_peer_mbed_dtls.h"
#include "stream_peer_mbedtls.h"

#ifdef TESTS_ENABLED
#include "tests/test_crypto_mbedtls.h"
#endif

void initialize_mbedtls_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	CryptoMbedTLS::initialize_crypto();
	StreamPeerMbedTLS::initialize_tls();
	PacketPeerMbedDTLS::initialize_dtls();
	DTLSServerMbedTLS::initialize();
}

void uninitialize_mbedtls_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	DTLSServerMbedTLS::finalize();
	PacketPeerMbedDTLS::finalize_dtls();
	StreamPeerMbedTLS::finalize_tls();
	CryptoMbedTLS::finalize_crypto();
}
