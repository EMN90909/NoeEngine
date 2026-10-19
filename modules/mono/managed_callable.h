// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef MANAGED_CALLABLE_H
#define MANAGED_CALLABLE_H

#include "core/os/mutex.h"
#include "core/templates/self_list.h"
#include "core/variant/callable.h"

#include "mono_gc_handle.h"

class ManagedCallable : public CallableCustom {
	friend class CSharpLanguage;
	GCHandleIntPtr delegate_handle;
	void *trampoline = nullptr;
	ObjectID object_id;

#ifdef GD_MONO_HOT_RELOAD
	SelfList<ManagedCallable> self_instance = this;
	static SelfList<ManagedCallable>::List instances;
	static RBMap<ManagedCallable *, Array> instances_pending_reload;
	static Mutex instances_mutex;
#endif

public:
	uint32_t hash() const override;
	String get_as_text() const override;
	CompareEqualFunc get_compare_equal_func() const override;
	CompareLessFunc get_compare_less_func() const override;
	ObjectID get_object() const override;
	void call(const Variant **p_arguments, int p_argcount, Variant &r_return_value, Callable::CallError &r_call_error) const override;

	_FORCE_INLINE_ GCHandleIntPtr get_delegate() const { return delegate_handle; }
	_FORCE_INLINE_ void *get_trampoline() const { return trampoline; }

	static bool compare_equal(const CallableCustom *p_a, const CallableCustom *p_b);
	static bool compare_less(const CallableCustom *p_a, const CallableCustom *p_b);

	static constexpr CompareEqualFunc compare_equal_func_ptr = &ManagedCallable::compare_equal;
	static constexpr CompareEqualFunc compare_less_func_ptr = &ManagedCallable::compare_less;

	void release_delegate_handle();

	ManagedCallable(GCHandleIntPtr p_delegate_handle, void *p_trampoline, ObjectID p_object_id);
	~ManagedCallable();
};

#endif // MANAGED_CALLABLE_H
