// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "panel.h"

void Panel::_update_theme_item_cache() {
	Control::_update_theme_item_cache();

	theme_cache.panel_style = get_theme_stylebox(SNAME("panel"));
}

void Panel::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_DRAW: {
			RID ci = get_canvas_item();
			theme_cache.panel_style->draw(ci, Rect2(Point2(), get_size()));
		} break;
	}
}

Panel::Panel() {
	// Has visible stylebox, so stop by default.
	set_mouse_filter(MOUSE_FILTER_STOP);
}
