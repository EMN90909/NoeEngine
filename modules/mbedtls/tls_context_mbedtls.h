// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef TLS_CONTEXT_MBEDTLS_H
#define TLS_CONTEXT_MBEDTLS_H

#include "crypto_mbedtls.h"

#include "core/io/file_access.h"

#include "core/object/ref_counted.h"

#include <mbedtls/config.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/debug.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ssl.h>
#include <mbedtls/ssl_cookie.h>

class TLSContextMbedTLS;

class CookieContextMbedTLS : public RefCounted {
	friend class TLSContextMbedTLS;

protected:
	bool inited = false;
	mbedtls_entropy_context entropy;
	mbedtls_ctr_drbg_context ctr_drbg;
	mbedtls_ssl_cookie_ctx cookie_ctx;

public:
	Error setup();
	void clear();

	CookieContextMbedTLS();
	~CookieContextMbedTLS();
};

class TLSContextMbedTLS : public RefCounted {
protected:
	bool inited = false;

public:
	static void print_mbedtls_error(int p_ret);

	Ref<X509CertificateMbedTLS> certs;
	Ref<CryptoKeyMbedTLS> pkey;
	Ref<CookieContextMbedTLS> cookies;

	mbedtls_entropy_context entropy;
	mbedtls_ctr_drbg_context ctr_drbg;
	mbedtls_ssl_context tls;
	mbedtls_ssl_config conf;

	Error _setup(int p_endpoint, int p_transport, int p_authmode);
	Error init_server(int p_transport, Ref<TLSOptions> p_options, Ref<CookieContextMbedTLS> p_cookies = Ref<CookieContextMbedTLS>());
	Error init_client(int p_transport, const String &p_hostname, Ref<TLSOptions> p_options);
	void clear();

	mbedtls_ssl_context *get_context();

	TLSContextMbedTLS();
	~TLSContextMbedTLS();
};

#endif // TLS_CONTEXT_MBEDTLS_H
