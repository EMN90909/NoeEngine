// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "callable_method_pointer.h"

bool CallableCustomMethodPointerBase::compare_equal(const CallableCustom *p_a, const CallableCustom *p_b) {
	const CallableCustomMethodPointerBase *a = static_cast<const CallableCustomMethodPointerBase *>(p_a);
	const CallableCustomMethodPointerBase *b = static_cast<const CallableCustomMethodPointerBase *>(p_b);

	if (a->comp_size != b->comp_size) {
		return false;
	}

	for (uint32_t i = 0; i < a->comp_size; i++) {
		if (a->comp_ptr[i] != b->comp_ptr[i]) {
			return false;
		}
	}

	return true;
}

bool CallableCustomMethodPointerBase::compare_less(const CallableCustom *p_a, const CallableCustom *p_b) {
	const CallableCustomMethodPointerBase *a = static_cast<const CallableCustomMethodPointerBase *>(p_a);
	const CallableCustomMethodPointerBase *b = static_cast<const CallableCustomMethodPointerBase *>(p_b);

	if (a->comp_size != b->comp_size) {
		return a->comp_size < b->comp_size;
	}

	for (uint32_t i = 0; i < a->comp_size; i++) {
		if (a->comp_ptr[i] == b->comp_ptr[i]) {
			continue;
		}

		return a->comp_ptr[i] < b->comp_ptr[i];
	}

	return false;
}

CallableCustom::CompareEqualFunc CallableCustomMethodPointerBase::get_compare_equal_func() const {
	return compare_equal;
}

CallableCustom::CompareLessFunc CallableCustomMethodPointerBase::get_compare_less_func() const {
	return compare_less;
}

uint32_t CallableCustomMethodPointerBase::hash() const {
	return h;
}

void CallableCustomMethodPointerBase::_setup(uint32_t *p_base_ptr, uint32_t p_ptr_size) {
	comp_ptr = p_base_ptr;
	comp_size = p_ptr_size / 4;

	// Precompute hash.
	for (uint32_t i = 0; i < comp_size; i++) {
		if (i == 0) {
			h = hash_murmur3_one_32(comp_ptr[i]);
		} else {
			h = hash_murmur3_one_32(comp_ptr[i], h);
		}
	}
}
