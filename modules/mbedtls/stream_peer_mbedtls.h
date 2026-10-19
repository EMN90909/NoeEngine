// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef STREAM_PEER_MBEDTLS_H
#define STREAM_PEER_MBEDTLS_H

#include "core/io/stream_peer_tls.h"
#include "tls_context_mbedtls.h"

class StreamPeerMbedTLS : public StreamPeerTLS {
private:
	Status status = STATUS_DISCONNECTED;
	String hostname;

	Ref<StreamPeer> base;

	static StreamPeerTLS *_create_func();

	static int bio_recv(void *ctx, unsigned char *buf, size_t len);
	static int bio_send(void *ctx, const unsigned char *buf, size_t len);
	void _cleanup();

protected:
	Ref<TLSContextMbedTLS> tls_ctx;

	Error _do_handshake();

public:
	virtual void poll();
	virtual Error accept_stream(Ref<StreamPeer> p_base, Ref<TLSOptions> p_options);
	virtual Error connect_to_stream(Ref<StreamPeer> p_base, const String &p_common_name, Ref<TLSOptions> p_options);
	virtual Status get_status() const;
	virtual Ref<StreamPeer> get_stream() const;

	virtual void disconnect_from_stream();

	virtual Error put_data(const uint8_t *p_data, int p_bytes);
	virtual Error put_partial_data(const uint8_t *p_data, int p_bytes, int &r_sent);

	virtual Error get_data(uint8_t *p_buffer, int p_bytes);
	virtual Error get_partial_data(uint8_t *p_buffer, int p_bytes, int &r_received);

	virtual int get_available_bytes() const;

	static void initialize_tls();
	static void finalize_tls();

	StreamPeerMbedTLS();
	~StreamPeerMbedTLS();
};

#endif // STREAM_PEER_MBEDTLS_H
