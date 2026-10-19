// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef CRASH_HANDLER_LINUXBSD_H
#define CRASH_HANDLER_LINUXBSD_H

class CrashHandler {
	bool disabled;

public:
	void initialize();

	void disable();
	bool is_disabled() const { return disabled; };

	CrashHandler();
	~CrashHandler();
};

#endif // CRASH_HANDLER_LINUXBSD_H
