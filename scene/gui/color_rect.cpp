// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "color_rect.h"

void ColorRect::set_color(const Color &p_color) {
	if (color == p_color) {
		return;
	}
	color = p_color;
	queue_redraw();
}

Color ColorRect::get_color() const {
	return color;
}

void ColorRect::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_DRAW: {
			draw_rect(Rect2(Point2(), get_size()), color);
		} break;
	}
}

void ColorRect::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_color", "color"), &ColorRect::set_color);
	ClassDB::bind_method(D_METHOD("get_color"), &ColorRect::get_color);

	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "color"), "set_color", "get_color");
}
