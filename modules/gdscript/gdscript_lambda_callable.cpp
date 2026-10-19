// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "gdscript_lambda_callable.h"

#include "core/templates/hashfuncs.h"
#include "gdscript.h"

bool GDScriptLambdaCallable::compare_equal(const CallableCustom *p_a, const CallableCustom *p_b) {
	// Lambda callables are only compared by reference.
	return p_a == p_b;
}

bool GDScriptLambdaCallable::compare_less(const CallableCustom *p_a, const CallableCustom *p_b) {
	// Lambda callables are only compared by reference.
	return p_a < p_b;
}

uint32_t GDScriptLambdaCallable::hash() const {
	return h;
}

String GDScriptLambdaCallable::get_as_text() const {
	if (function->get_name() != StringName()) {
		return function->get_name().operator String() + "(lambda)";
	}
	return "(anonymous lambda)";
}

CallableCustom::CompareEqualFunc GDScriptLambdaCallable::get_compare_equal_func() const {
	return compare_equal;
}

CallableCustom::CompareLessFunc GDScriptLambdaCallable::get_compare_less_func() const {
	return compare_less;
}

ObjectID GDScriptLambdaCallable::get_object() const {
	return script->get_instance_id();
}

void GDScriptLambdaCallable::call(const Variant **p_arguments, int p_argcount, Variant &r_return_value, Callable::CallError &r_call_error) const {
	int captures_amount = captures.size();

	if (captures_amount > 0) {
		Vector<const Variant *> args;
		args.resize(p_argcount + captures_amount);
		for (int i = 0; i < captures_amount; i++) {
			args.write[i] = &captures[i];
		}
		for (int i = 0; i < p_argcount; i++) {
			args.write[i + captures_amount] = p_arguments[i];
		}

		r_return_value = function->call(nullptr, args.ptrw(), args.size(), r_call_error);
		r_call_error.argument -= captures_amount;
	} else {
		r_return_value = function->call(nullptr, p_arguments, p_argcount, r_call_error);
	}
}

GDScriptLambdaCallable::GDScriptLambdaCallable(Ref<GDScript> p_script, GDScriptFunction *p_function, const Vector<Variant> &p_captures) {
	script = p_script;
	function = p_function;
	captures = p_captures;

	h = (uint32_t)hash_murmur3_one_64((uint64_t)this);
}

bool GDScriptLambdaSelfCallable::compare_equal(const CallableCustom *p_a, const CallableCustom *p_b) {
	// Lambda callables are only compared by reference.
	return p_a == p_b;
}

bool GDScriptLambdaSelfCallable::compare_less(const CallableCustom *p_a, const CallableCustom *p_b) {
	// Lambda callables are only compared by reference.
	return p_a < p_b;
}

uint32_t GDScriptLambdaSelfCallable::hash() const {
	return h;
}

String GDScriptLambdaSelfCallable::get_as_text() const {
	if (function->get_name() != StringName()) {
		return function->get_name().operator String() + "(lambda)";
	}
	return "(anonymous lambda)";
}

CallableCustom::CompareEqualFunc GDScriptLambdaSelfCallable::get_compare_equal_func() const {
	return compare_equal;
}

CallableCustom::CompareLessFunc GDScriptLambdaSelfCallable::get_compare_less_func() const {
	return compare_less;
}

ObjectID GDScriptLambdaSelfCallable::get_object() const {
	return object->get_instance_id();
}

void GDScriptLambdaSelfCallable::call(const Variant **p_arguments, int p_argcount, Variant &r_return_value, Callable::CallError &r_call_error) const {
#ifdef DEBUG_ENABLED
	if (object->get_script_instance() == nullptr || object->get_script_instance()->get_language() != GDScriptLanguage::get_singleton()) {
		ERR_PRINT("Trying to call a lambda with an invalid instance.");
		r_call_error.error = Callable::CallError::CALL_ERROR_INSTANCE_IS_NULL;
		return;
	}
#endif

	int captures_amount = captures.size();

	if (captures_amount > 0) {
		Vector<const Variant *> args;
		args.resize(p_argcount + captures_amount);
		for (int i = 0; i < captures_amount; i++) {
			args.write[i] = &captures[i];
		}
		for (int i = 0; i < p_argcount; i++) {
			args.write[i + captures_amount] = p_arguments[i];
		}

		r_return_value = function->call(static_cast<GDScriptInstance *>(object->get_script_instance()), args.ptrw(), args.size(), r_call_error);
		r_call_error.argument -= captures_amount;
	} else {
		r_return_value = function->call(static_cast<GDScriptInstance *>(object->get_script_instance()), p_arguments, p_argcount, r_call_error);
	}
}

GDScriptLambdaSelfCallable::GDScriptLambdaSelfCallable(Ref<RefCounted> p_self, GDScriptFunction *p_function, const Vector<Variant> &p_captures) {
	reference = p_self;
	object = p_self.ptr();
	function = p_function;
	captures = p_captures;

	h = (uint32_t)hash_murmur3_one_64((uint64_t)this);
}

GDScriptLambdaSelfCallable::GDScriptLambdaSelfCallable(Object *p_self, GDScriptFunction *p_function, const Vector<Variant> &p_captures) {
	object = p_self;
	function = p_function;
	captures = p_captures;

	h = (uint32_t)hash_murmur3_one_64((uint64_t)this);
}
