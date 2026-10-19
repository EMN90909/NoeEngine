// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef FREEDESKTOP_PORTAL_DESKTOP_H
#define FREEDESKTOP_PORTAL_DESKTOP_H

#ifdef DBUS_ENABLED

#include <stdint.h>

class FreeDesktopPortalDesktop {
private:
	bool unsupported = false;

	// Read a setting from org.freekdesktop.portal.Settings
	bool read_setting(const char *p_namespace, const char *p_key, int p_type, void *r_value);

public:
	FreeDesktopPortalDesktop();

	bool is_supported() { return !unsupported; }

	// Retrieve the system's preferred color scheme.
	// 0: No preference or unknown.
	// 1: Prefer dark appearance.
	// 2: Prefer light appearance.
	uint32_t get_appearance_color_scheme();
};

#endif // DBUS_ENABLED

#endif // FREEDESKTOP_PORTAL_DESKTOP_H
