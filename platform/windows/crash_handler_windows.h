// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef CRASH_HANDLER_WINDOWS_H
#define CRASH_HANDLER_WINDOWS_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// Crash handler exception only enabled with MSVC
#if defined(DEBUG_ENABLED) && defined(MSVC)
#define CRASH_HANDLER_EXCEPTION 1

extern DWORD CrashHandlerException(EXCEPTION_POINTERS *ep);
#endif

class CrashHandler {
	bool disabled;

public:
	void initialize();

	void disable();
	bool is_disabled() const { return disabled; };

	CrashHandler();
	~CrashHandler();
};

#endif // CRASH_HANDLER_WINDOWS_H
