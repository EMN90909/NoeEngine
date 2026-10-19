// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "separator.h"

Size2 Separator::get_minimum_size() const {
	Size2 ms(3, 3);
	if (orientation == VERTICAL) {
		ms.x = theme_cache.separation;
	} else { // HORIZONTAL
		ms.y = theme_cache.separation;
	}
	return ms;
}

void Separator::_update_theme_item_cache() {
	Control::_update_theme_item_cache();

	theme_cache.separation = get_theme_constant(SNAME("separation"));
	theme_cache.separator_style = get_theme_stylebox(SNAME("separator"));
}

void Separator::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_DRAW: {
			Size2i size = get_size();
			Size2i ssize = theme_cache.separator_style->get_minimum_size();

			if (orientation == VERTICAL) {
				theme_cache.separator_style->draw(get_canvas_item(), Rect2((size.x - ssize.x) / 2, 0, ssize.x, size.y));
			} else {
				theme_cache.separator_style->draw(get_canvas_item(), Rect2(0, (size.y - ssize.y) / 2, size.x, ssize.y));
			}
		} break;
	}
}

Separator::Separator() {
}

Separator::~Separator() {
}

HSeparator::HSeparator() {
	orientation = HORIZONTAL;
}

VSeparator::VSeparator() {
	orientation = VERTICAL;
}
