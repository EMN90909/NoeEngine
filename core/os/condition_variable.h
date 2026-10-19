// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef CONDITION_VARIABLE_H
#define CONDITION_VARIABLE_H

#include <condition_variable>

// An object one or multiple threads can wait on a be notified by some other.
// Normally, you want to use a semaphore for such scenarios, but when the
// condition is something different than a count being greater than zero
// (which is the built-in logic in a semaphore) or you want to provide your
// own mutex to tie the wait-notify to some other behavior, you need to use this.

class ConditionVariable {
	mutable std::condition_variable condition;

public:
	template <class BinaryMutexT>
	_ALWAYS_INLINE_ void wait(const MutexLock<BinaryMutexT> &p_lock) const {
		condition.wait(const_cast<std::unique_lock<std::mutex> &>(p_lock.lock));
	}

	_ALWAYS_INLINE_ void notify_one() const {
		condition.notify_one();
	}

	_ALWAYS_INLINE_ void notify_all() const {
		condition.notify_all();
	}
};

#endif // CONDITION_VARIABLE_H
