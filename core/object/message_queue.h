// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include "core/object/object_id.h"
#include "core/os/thread_safe.h"
#include "core/variant/variant.h"

class Object;

class MessageQueue {
	_THREAD_SAFE_CLASS_

	enum {
		DEFAULT_QUEUE_SIZE_KB = 4096
	};

	enum {
		TYPE_CALL,
		TYPE_NOTIFICATION,
		TYPE_SET,
		FLAG_SHOW_ERROR = 1 << 14,
		FLAG_MASK = FLAG_SHOW_ERROR - 1

	};

	struct Message {
		Callable callable;
		int16_t type = 0;
		union {
			int16_t notification = 0;
			int16_t args;
		};
	};

	uint8_t *buffer = nullptr;
	uint32_t buffer_end = 0;
	uint32_t buffer_max_used = 0;
	uint32_t buffer_size = 0;

	void _call_function(const Callable &p_callable, const Variant *p_args, int p_argcount, bool p_show_error);

	static MessageQueue *singleton;

	bool flushing = false;

public:
	static MessageQueue *get_singleton();

	Error push_callp(ObjectID p_id, const StringName &p_method, const Variant **p_args, int p_argcount, bool p_show_error = false);
	template <typename... VarArgs>
	Error push_call(ObjectID p_id, const StringName &p_method, VarArgs... p_args) {
		Variant args[sizeof...(p_args) + 1] = { p_args..., Variant() }; // +1 makes sure zero sized arrays are also supported.
		const Variant *argptrs[sizeof...(p_args) + 1];
		for (uint32_t i = 0; i < sizeof...(p_args); i++) {
			argptrs[i] = &args[i];
		}
		return push_callp(p_id, p_method, sizeof...(p_args) == 0 ? nullptr : (const Variant **)argptrs, sizeof...(p_args));
	}

	Error push_notification(ObjectID p_id, int p_notification);
	Error push_set(ObjectID p_id, const StringName &p_prop, const Variant &p_value);
	Error push_callablep(const Callable &p_callable, const Variant **p_args, int p_argcount, bool p_show_error = false);

	template <typename... VarArgs>
	Error push_callable(const Callable &p_callable, VarArgs... p_args) {
		Variant args[sizeof...(p_args) + 1] = { p_args..., Variant() }; // +1 makes sure zero sized arrays are also supported.
		const Variant *argptrs[sizeof...(p_args) + 1];
		for (uint32_t i = 0; i < sizeof...(p_args); i++) {
			argptrs[i] = &args[i];
		}
		return push_callablep(p_callable, sizeof...(p_args) == 0 ? nullptr : (const Variant **)argptrs, sizeof...(p_args));
	}

	Error push_callp(Object *p_object, const StringName &p_method, const Variant **p_args, int p_argcount, bool p_show_error = false);
	template <typename... VarArgs>
	Error push_call(Object *p_object, const StringName &p_method, VarArgs... p_args) {
		Variant args[sizeof...(p_args) + 1] = { p_args..., Variant() }; // +1 makes sure zero sized arrays are also supported.
		const Variant *argptrs[sizeof...(p_args) + 1];
		for (uint32_t i = 0; i < sizeof...(p_args); i++) {
			argptrs[i] = &args[i];
		}
		return push_callp(p_object, p_method, sizeof...(p_args) == 0 ? nullptr : (const Variant **)argptrs, sizeof...(p_args));
	}

	Error push_notification(Object *p_object, int p_notification);
	Error push_set(Object *p_object, const StringName &p_prop, const Variant &p_value);

	void statistics();
	void flush();

	bool is_flushing() const;

	int get_max_buffer_usage() const;

	MessageQueue();
	~MessageQueue();
};

#endif // MESSAGE_QUEUE_H
