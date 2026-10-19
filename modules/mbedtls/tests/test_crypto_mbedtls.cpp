// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "test_crypto_mbedtls.h"

#include "modules/mbedtls/crypto_mbedtls.h"
#include "tests/test_macros.h"

namespace TestCryptoMbedTLS {

void hmac_digest_test(HashingContext::HashType ht, String expected_hex) {
	CryptoMbedTLS crypto;
	PackedByteArray key = String("supersecretkey").to_utf8_buffer();
	PackedByteArray msg = String("Return of the MAC!").to_utf8_buffer();
	PackedByteArray digest = crypto.hmac_digest(ht, key, msg);
	String hex = String::hex_encode_buffer(digest.ptr(), digest.size());
	CHECK(hex == expected_hex);
}

void hmac_context_digest_test(HashingContext::HashType ht, String expected_hex) {
	HMACContextMbedTLS ctx;
	PackedByteArray key = String("supersecretkey").to_utf8_buffer();
	PackedByteArray msg1 = String("Return of ").to_utf8_buffer();
	PackedByteArray msg2 = String("the MAC!").to_utf8_buffer();
	Error err = ctx.start(ht, key);
	CHECK(err == OK);
	err = ctx.update(msg1);
	CHECK(err == OK);
	err = ctx.update(msg2);
	CHECK(err == OK);
	PackedByteArray digest = ctx.finish();
	String hex = String::hex_encode_buffer(digest.ptr(), digest.size());
	CHECK(hex == expected_hex);
}
} // namespace TestCryptoMbedTLS
