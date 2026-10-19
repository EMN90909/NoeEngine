// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef THREADED_ARRAY_PROCESSOR_H
#define THREADED_ARRAY_PROCESSOR_H

#include "core/os/os.h"
#include "core/os/thread.h"
#include "core/os/thread_safe.h"
#include "core/templates/safe_refcount.h"

template <class C, class U>
struct ThreadArrayProcessData {
	uint32_t elements;
	SafeNumeric<uint32_t> index;
	C *instance;
	U userdata;
	void (C::*method)(uint32_t, U);

	void process(uint32_t p_index) {
		(instance->*method)(p_index, userdata);
	}
};

template <class T>
void process_array_thread(void *ud) {
	T &data = *(T *)ud;
	while (true) {
		uint32_t index = data.index.increment();
		if (index >= data.elements) {
			break;
		}
		data.process(index);
	}
}

template <class C, class M, class U>
void thread_process_array(uint32_t p_elements, C *p_instance, M p_method, U p_userdata) {
	ThreadArrayProcessData<C, U> data;
	data.method = p_method;
	data.instance = p_instance;
	data.userdata = p_userdata;
	data.index.set(0);
	data.elements = p_elements;
	data.process(0); //process first, let threads increment for next

	int thread_count = OS::get_singleton()->get_processor_count();
	Thread *threads = memnew_arr(Thread, thread_count);

	for (int i = 0; i < thread_count; i++) {
		threads[i].start(process_array_thread<ThreadArrayProcessData<C, U>>, &data);
	}

	for (int i = 0; i < thread_count; i++) {
		threads[i].wait_to_finish();
	}
	memdelete_arr(threads);
}

#endif // THREADED_ARRAY_PROCESSOR_H
