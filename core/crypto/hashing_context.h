// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef HASHING_CONTEXT_H
#define HASHING_CONTEXT_H

#include "core/object/ref_counted.h"

class HashingContext : public RefCounted {
	GDCLASS(HashingContext, RefCounted);

public:
	enum HashType {
		HASH_MD5,
		HASH_SHA1,
		HASH_SHA256
	};

private:
	void *ctx = nullptr;
	HashType type = HASH_MD5;

protected:
	static void _bind_methods();
	void _create_ctx(HashType p_type);
	void _delete_ctx();

public:
	Error start(HashType p_type);
	Error update(PackedByteArray p_chunk);
	PackedByteArray finish();

	HashingContext() {}
	~HashingContext();
};

VARIANT_ENUM_CAST(HashingContext::HashType);

#endif // HASHING_CONTEXT_H
