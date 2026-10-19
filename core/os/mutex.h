// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef MUTEX_H
#define MUTEX_H

#include "core/error/error_macros.h"
#include "core/typedefs.h"

#include <mutex>

template <class MutexT>
class MutexLock;

template <class StdMutexT>
class MutexImpl {
	friend class MutexLock<MutexImpl<StdMutexT>>;

	using StdMutexType = StdMutexT;

	mutable StdMutexT mutex;

public:
	_ALWAYS_INLINE_ void lock() const {
		mutex.lock();
	}

	_ALWAYS_INLINE_ void unlock() const {
		mutex.unlock();
	}

	_ALWAYS_INLINE_ bool try_lock() const {
		return mutex.try_lock();
	}
};

// A very special kind of mutex, used in scenarios where these
// requirements hold at the same time:
// - Must be used with a condition variable (only binary mutexes are suitable).
// - Must have recursive semnantics (or simulate, as this one does).
// The implementation keeps the lock count in TS. Therefore, only
// one object of each version of the template can exists; hence the Tag argument.
// Tags must be unique across the Godot codebase.
// Also, don't forget to declare the thread_local variable on each use.
template <int Tag>
class SafeBinaryMutex {
	friend class MutexLock<SafeBinaryMutex>;

	using StdMutexType = std::mutex;

	mutable std::mutex mutex;
	static thread_local uint32_t count;

public:
	_ALWAYS_INLINE_ void lock() const {
		if (++count == 1) {
			mutex.lock();
		}
	}

	_ALWAYS_INLINE_ void unlock() const {
		DEV_ASSERT(count);
		if (--count == 0) {
			mutex.unlock();
		}
	}

	_ALWAYS_INLINE_ bool try_lock() const {
		if (count) {
			count++;
			return true;
		} else {
			if (mutex.try_lock()) {
				count++;
				return true;
			} else {
				return false;
			}
		}
	}

	~SafeBinaryMutex() {
		DEV_ASSERT(!count);
	}
};

template <class MutexT>
class MutexLock {
	friend class ConditionVariable;

	std::unique_lock<typename MutexT::StdMutexType> lock;

public:
	_ALWAYS_INLINE_ explicit MutexLock(const MutexT &p_mutex) :
			lock(p_mutex.mutex) {
	}
};

using Mutex = MutexImpl<std::recursive_mutex>; // Recursive, for general use
using BinaryMutex = MutexImpl<std::mutex>; // Non-recursive, handle with care

extern template class MutexImpl<std::recursive_mutex>;
extern template class MutexImpl<std::mutex>;
extern template class MutexLock<MutexImpl<std::recursive_mutex>>;
extern template class MutexLock<MutexImpl<std::mutex>>;

#endif // MUTEX_H
