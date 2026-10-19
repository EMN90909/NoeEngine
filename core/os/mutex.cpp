// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "mutex.h"

static Mutex _global_mutex;

void _global_lock() {
	_global_mutex.lock();
}

void _global_unlock() {
	_global_mutex.unlock();
}

template class MutexImpl<std::recursive_mutex>;
template class MutexImpl<std::mutex>;
template class MutexLock<MutexImpl<std::recursive_mutex>>;
template class MutexLock<MutexImpl<std::mutex>>;
