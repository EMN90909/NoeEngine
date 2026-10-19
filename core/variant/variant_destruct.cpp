// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "variant_destruct.h"

#include "core/templates/local_vector.h"

static Variant::PTRDestructor destruct_pointers[Variant::VARIANT_MAX] = { nullptr };

template <class T>
static void add_destructor() {
	destruct_pointers[T::get_base_type()] = T::ptr_destruct;
}

void Variant::_register_variant_destructors() {
	add_destructor<VariantDestruct<String>>();
	add_destructor<VariantDestruct<StringName>>();
	add_destructor<VariantDestruct<NodePath>>();
	add_destructor<VariantDestruct<Callable>>();
	add_destructor<VariantDestruct<Signal>>();
	add_destructor<VariantDestruct<Dictionary>>();
	add_destructor<VariantDestruct<Array>>();
	add_destructor<VariantDestruct<PackedByteArray>>();
	add_destructor<VariantDestruct<PackedInt32Array>>();
	add_destructor<VariantDestruct<PackedInt64Array>>();
	add_destructor<VariantDestruct<PackedFloat32Array>>();
	add_destructor<VariantDestruct<PackedFloat64Array>>();
	add_destructor<VariantDestruct<PackedStringArray>>();
	add_destructor<VariantDestruct<PackedVector2Array>>();
	add_destructor<VariantDestruct<PackedVector3Array>>();
	add_destructor<VariantDestruct<PackedColorArray>>();
}

void Variant::_unregister_variant_destructors() {
	// Nothing to be done.
}

Variant::PTRDestructor Variant::get_ptr_destructor(Variant::Type p_type) {
	ERR_FAIL_INDEX_V(p_type, Variant::VARIANT_MAX, nullptr);
	return destruct_pointers[p_type];
}

bool Variant::has_destructor(Variant::Type p_type) {
	ERR_FAIL_INDEX_V(p_type, Variant::VARIANT_MAX, false);
	return destruct_pointers[p_type] != nullptr;
}
