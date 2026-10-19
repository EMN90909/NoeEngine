// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef FREEDESKTOP_SCREENSAVER_H
#define FREEDESKTOP_SCREENSAVER_H

#ifdef DBUS_ENABLED

#include <stdint.h>

class FreeDesktopScreenSaver {
private:
	uint32_t cookie = 0;
	bool unsupported = false;

public:
	FreeDesktopScreenSaver();
	void inhibit();
	void uninhibit();
};

#endif // DBUS_ENABLED

#endif // FREEDESKTOP_SCREENSAVER_H
