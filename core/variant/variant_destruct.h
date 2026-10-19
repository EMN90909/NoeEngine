// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef VARIANT_DESTRUCT_H
#define VARIANT_DESTRUCT_H

#include "core/variant/variant.h"

#include "core/object/class_db.h"

template <class T>
struct VariantDestruct {};

#define MAKE_PTRDESTRUCT(m_type)                               \
	template <>                                                \
	struct VariantDestruct<m_type> {                           \
		_FORCE_INLINE_ static void ptr_destruct(void *p_ptr) { \
			reinterpret_cast<m_type *>(p_ptr)->~m_type();      \
		}                                                      \
		_FORCE_INLINE_ static Variant::Type get_base_type() {  \
			return GetTypeInfo<m_type>::VARIANT_TYPE;          \
		}                                                      \
	}

MAKE_PTRDESTRUCT(String);
MAKE_PTRDESTRUCT(StringName);
MAKE_PTRDESTRUCT(NodePath);
MAKE_PTRDESTRUCT(Callable);
MAKE_PTRDESTRUCT(Signal);
MAKE_PTRDESTRUCT(Dictionary);
MAKE_PTRDESTRUCT(Array);
MAKE_PTRDESTRUCT(PackedByteArray);
MAKE_PTRDESTRUCT(PackedInt32Array);
MAKE_PTRDESTRUCT(PackedInt64Array);
MAKE_PTRDESTRUCT(PackedFloat32Array);
MAKE_PTRDESTRUCT(PackedFloat64Array);
MAKE_PTRDESTRUCT(PackedStringArray);
MAKE_PTRDESTRUCT(PackedVector2Array);
MAKE_PTRDESTRUCT(PackedVector3Array);
MAKE_PTRDESTRUCT(PackedColorArray);

#undef MAKE_PTRDESTRUCT

#endif // VARIANT_DESTRUCT_H
