// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "ref_counted.h"

#include "core/object/script_language.h"

bool RefCounted::init_ref() {
	if (reference()) {
		if (!is_referenced() && refcount_init.unref()) {
			unreference(); // first referencing is already 1, so compensate for the ref above
		}

		return true;
	} else {
		return false;
	}
}

void RefCounted::_bind_methods() {
	ClassDB::bind_method(D_METHOD("init_ref"), &RefCounted::init_ref);
	ClassDB::bind_method(D_METHOD("reference"), &RefCounted::reference);
	ClassDB::bind_method(D_METHOD("unreference"), &RefCounted::unreference);
	ClassDB::bind_method(D_METHOD("get_reference_count"), &RefCounted::get_reference_count);
}

int RefCounted::get_reference_count() const {
	return refcount.get();
}

bool RefCounted::reference() {
	uint32_t rc_val = refcount.refval();
	bool success = rc_val != 0;

	if (success && rc_val <= 2 /* higher is not relevant */) {
		if (get_script_instance()) {
			get_script_instance()->refcount_incremented();
		}
		if (_get_extension() && _get_extension()->reference) {
			_get_extension()->reference(_get_extension_instance());
		}

		_instance_binding_reference(true);
	}

	return success;
}

bool RefCounted::unreference() {
	uint32_t rc_val = refcount.unrefval();
	bool die = rc_val == 0;

	if (rc_val <= 1 /* higher is not relevant */) {
		if (get_script_instance()) {
			bool script_ret = get_script_instance()->refcount_decremented();
			die = die && script_ret;
		}
		if (_get_extension() && _get_extension()->unreference) {
			_get_extension()->unreference(_get_extension_instance());
		}

		bool binding_ret = _instance_binding_reference(false);
		die = die && binding_ret;
	}

	return die;
}

RefCounted::RefCounted() :
		Object(true) {
	refcount.init();
	refcount_init.init();
}

Variant WeakRef::get_ref() const {
	if (ref.is_null()) {
		return Variant();
	}

	Object *obj = ObjectDB::get_instance(ref);
	if (!obj) {
		return Variant();
	}
	RefCounted *r = cast_to<RefCounted>(obj);
	if (r) {
		return Ref<RefCounted>(r);
	}

	return obj;
}

void WeakRef::set_obj(Object *p_object) {
	ref = p_object ? p_object->get_instance_id() : ObjectID();
}

void WeakRef::set_ref(const Ref<RefCounted> &p_ref) {
	ref = p_ref.is_valid() ? p_ref->get_instance_id() : ObjectID();
}

void WeakRef::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_ref"), &WeakRef::get_ref);
}
