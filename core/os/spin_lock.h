// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef SPIN_LOCK_H
#define SPIN_LOCK_H

#include "core/typedefs.h"

#include <atomic>

class SpinLock {
	std::atomic_flag locked = ATOMIC_FLAG_INIT;

public:
	_ALWAYS_INLINE_ void lock() {
		while (locked.test_and_set(std::memory_order_acquire)) {
			// Continue.
		}
	}
	_ALWAYS_INLINE_ void unlock() {
		locked.clear(std::memory_order_release);
	}
};

#endif // SPIN_LOCK_H
