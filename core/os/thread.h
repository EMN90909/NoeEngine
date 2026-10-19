// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "platform_config.h"
// Define PLATFORM_THREAD_OVERRIDE in your platform's `platform_config.h`
// to use a custom Thread implementation defined in `platform/[your_platform]/platform_thread.h`
// Overriding the platform implementation is required in some proprietary platforms
#ifdef PLATFORM_THREAD_OVERRIDE
#include "platform_thread.h"
#else

#ifndef THREAD_H
#define THREAD_H

#include "core/templates/safe_refcount.h"
#include "core/typedefs.h"

#include <thread>

class String;

class Thread {
public:
	typedef void (*Callback)(void *p_userdata);

	typedef uint64_t ID;

	enum Priority {
		PRIORITY_LOW,
		PRIORITY_NORMAL,
		PRIORITY_HIGH
	};

	struct Settings {
		Priority priority;
		Settings() { priority = PRIORITY_NORMAL; }
	};

	struct PlatformFunctions {
		Error (*set_name)(const String &) = nullptr;
		void (*set_priority)(Thread::Priority) = nullptr;
		void (*init)() = nullptr;
		void (*wrapper)(Thread::Callback, void *) = nullptr;
		void (*term)() = nullptr;
	};

private:
	friend class Main;

	static ID main_thread_id;

	static uint64_t _thread_id_hash(const std::thread::id &p_t);

	ID id = _thread_id_hash(std::thread::id());
	static thread_local ID caller_id;
	std::thread thread;

	static void callback(ID p_caller_id, const Settings &p_settings, Thread::Callback p_callback, void *p_userdata);

	static PlatformFunctions platform_functions;

public:
	static void _set_platform_functions(const PlatformFunctions &p_functions);

	_FORCE_INLINE_ ID get_id() const { return id; }
	// get the ID of the caller thread
	_FORCE_INLINE_ static ID get_caller_id() { return caller_id; }
	// get the ID of the main thread
	_FORCE_INLINE_ static ID get_main_id() { return main_thread_id; }

	static Error set_name(const String &p_name);

	void start(Thread::Callback p_callback, void *p_user, const Settings &p_settings = Settings());
	bool is_started() const;
	///< waits until thread is finished, and deallocates it.
	void wait_to_finish();

	Thread();
	~Thread();
};

#endif // THREAD_H

#endif // PLATFORM_THREAD_OVERRIDE
