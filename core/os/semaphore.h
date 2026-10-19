// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "core/error/error_list.h"
#include "core/typedefs.h"

#include <condition_variable>
#include <mutex>

class Semaphore {
private:
	mutable std::mutex mutex;
	mutable std::condition_variable condition;
	mutable uint32_t count = 0; // Initialized as locked.

public:
	_ALWAYS_INLINE_ void post() const {
		std::lock_guard lock(mutex);
		count++;
		condition.notify_one();
	}

	_ALWAYS_INLINE_ void wait() const {
		std::unique_lock lock(mutex);
		while (!count) { // Handle spurious wake-ups.
			condition.wait(lock);
		}
		count--;
	}

	_ALWAYS_INLINE_ bool try_wait() const {
		std::lock_guard lock(mutex);
		if (count) {
			count--;
			return true;
		} else {
			return false;
		}
	}
};

#endif // SEMAPHORE_H
