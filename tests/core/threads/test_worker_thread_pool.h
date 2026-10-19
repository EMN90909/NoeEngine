// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef TEST_WORKER_THREAD_POOL_H
#define TEST_WORKER_THREAD_POOL_H

#include "core/object/worker_thread_pool.h"

#include "tests/test_macros.h"

namespace TestWorkerThreadPool {

int u32scmp(const char32_t *l, const char32_t *r) {
	for (; *l == *r && *l && *r; l++, r++) {
		// Continue.
	}
	return *l - *r;
}

static void static_test(void *p_arg) {
	SafeNumeric<uint32_t> *counter = (SafeNumeric<uint32_t> *)p_arg;
	counter->increment();
}

static SafeNumeric<uint32_t> callable_counter;

static void static_callable_test() {
	callable_counter.increment();
}

TEST_CASE("[WorkerThreadPool] Process 256 threads using native task") {
	const int count = 256;
	SafeNumeric<uint32_t> counter;
	WorkerThreadPool::TaskID tasks[count];
	for (int i = 0; i < count; i++) {
		tasks[i] = WorkerThreadPool::get_singleton()->add_native_task(static_test, &counter, true);
	}
	for (int i = 0; i < count; i++) {
		WorkerThreadPool::get_singleton()->wait_for_task_completion(tasks[i]);
	}

	CHECK(counter.get() == count);
}

TEST_CASE("[WorkerThreadPool] Process 256 threads using native low priority") {
	const int count = 256;
	SafeNumeric<uint32_t> counter = SafeNumeric<uint32_t>(0);
	WorkerThreadPool::TaskID tasks[count];
	for (int i = 0; i < count; i++) {
		tasks[i] = WorkerThreadPool::get_singleton()->add_native_task(static_test, &counter, false);
	}
	for (int i = 0; i < count; i++) {
		WorkerThreadPool::get_singleton()->wait_for_task_completion(tasks[i]);
	}

	CHECK(counter.get() == count);
}

TEST_CASE("[WorkerThreadPool] Process 256 threads using callable") {
	const int count = 256;
	WorkerThreadPool::TaskID tasks[count];
	callable_counter.set(0);
	for (int i = 0; i < count; i++) {
		tasks[i] = WorkerThreadPool::get_singleton()->add_task(callable_mp_static(static_callable_test), true);
	}
	for (int i = 0; i < count; i++) {
		WorkerThreadPool::get_singleton()->wait_for_task_completion(tasks[i]);
	}

	CHECK(callable_counter.get() == count);
}

TEST_CASE("[WorkerThreadPool] Process 256 threads using callable low priority") {
	const int count = 256;
	WorkerThreadPool::TaskID tasks[count];
	callable_counter.set(0);
	for (int i = 0; i < count; i++) {
		tasks[i] = WorkerThreadPool::get_singleton()->add_task(callable_mp_static(static_callable_test), false);
	}
	for (int i = 0; i < count; i++) {
		WorkerThreadPool::get_singleton()->wait_for_task_completion(tasks[i]);
	}

	CHECK(callable_counter.get() == count);
}

static void static_group_test(void *p_arg, uint32_t p_index) {
	SafeNumeric<uint32_t> *counter = (SafeNumeric<uint32_t> *)p_arg;
	counter->exchange_if_greater(p_index);
}

TEST_CASE("[WorkerThreadPool] Process 256 elements on native task group") {
	const int count = 256;
	SafeNumeric<uint32_t> counter;
	WorkerThreadPool::GroupID group = WorkerThreadPool::get_singleton()->add_native_group_task(static_group_test, &counter, count, -1, true);
	WorkerThreadPool::get_singleton()->wait_for_group_task_completion(group);
	CHECK(counter.get() == count - 1);
}

TEST_CASE("[WorkerThreadPool] Process 256 elements on native task group low priority") {
	const int count = 256;
	SafeNumeric<uint32_t> counter;
	WorkerThreadPool::GroupID group = WorkerThreadPool::get_singleton()->add_native_group_task(static_group_test, &counter, count, -1, false);
	WorkerThreadPool::get_singleton()->wait_for_group_task_completion(group);
	CHECK(counter.get() == count - 1);
}

static SafeNumeric<uint32_t> callable_group_counter;

static void static_callable_group_test(uint32_t p_index) {
	callable_group_counter.exchange_if_greater(p_index);
}

TEST_CASE("[WorkerThreadPool] Process 256 elements on native task group") {
	const int count = 256;
	WorkerThreadPool::GroupID group = WorkerThreadPool::get_singleton()->add_group_task(callable_mp_static(static_callable_group_test), count, -1, true);
	WorkerThreadPool::get_singleton()->wait_for_group_task_completion(group);
	CHECK(callable_group_counter.get() == count - 1);
}

TEST_CASE("[WorkerThreadPool] Process 256 elements on native task group low priority") {
	const int count = 256;
	callable_group_counter.set(0);
	WorkerThreadPool::GroupID group = WorkerThreadPool::get_singleton()->add_group_task(callable_mp_static(static_callable_group_test), count, -1, false);
	WorkerThreadPool::get_singleton()->wait_for_group_task_completion(group);
	CHECK(callable_group_counter.get() == count - 1);
}

} // namespace TestWorkerThreadPool

#endif // TEST_WORKER_THREAD_POOL_H
