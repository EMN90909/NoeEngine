// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef CANVAS_MODULATE_H
#define CANVAS_MODULATE_H

#include "scene/2d/node_2d.h"

class CanvasModulate : public Node2D {
	GDCLASS(CanvasModulate, Node2D);

	Color color = Color(1, 1, 1, 1);

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	void set_color(const Color &p_color);
	Color get_color() const;

	PackedStringArray get_configuration_warnings() const override;

	CanvasModulate();
	~CanvasModulate();
};

#endif // CANVAS_MODULATE_H
