// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef AES_CONTEXT_H
#define AES_CONTEXT_H

#include "core/crypto/crypto_core.h"
#include "core/object/ref_counted.h"

class AESContext : public RefCounted {
	GDCLASS(AESContext, RefCounted);

public:
	enum Mode {
		MODE_ECB_ENCRYPT,
		MODE_ECB_DECRYPT,
		MODE_CBC_ENCRYPT,
		MODE_CBC_DECRYPT,
		MODE_MAX
	};

private:
	Mode mode = MODE_MAX;
	CryptoCore::AESContext ctx;
	PackedByteArray iv;

protected:
	static void _bind_methods();

public:
	Error start(Mode p_mode, PackedByteArray p_key, PackedByteArray p_iv = PackedByteArray());
	PackedByteArray update(PackedByteArray p_src);
	PackedByteArray get_iv_state();
	void finish();

	AESContext();
};

VARIANT_ENUM_CAST(AESContext::Mode);

#endif // AES_CONTEXT_H
