// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef THREAD_SAFE_H
#define THREAD_SAFE_H

#include "core/os/mutex.h"

#define _THREAD_SAFE_CLASS_ mutable Mutex _thread_safe_;
#define _THREAD_SAFE_METHOD_ MutexLock _thread_safe_method_(_thread_safe_);
#define _THREAD_SAFE_LOCK_ _thread_safe_.lock();
#define _THREAD_SAFE_UNLOCK_ _thread_safe_.unlock();

#endif // THREAD_SAFE_H
