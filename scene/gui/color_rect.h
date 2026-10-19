// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef COLOR_RECT_H
#define COLOR_RECT_H

#include "scene/gui/control.h"

class ColorRect : public Control {
	GDCLASS(ColorRect, Control);

	Color color = Color(1, 1, 1);

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	void set_color(const Color &p_color);
	Color get_color() const;
};

#endif // COLOR_RECT_H
