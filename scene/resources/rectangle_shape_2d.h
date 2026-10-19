// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef RECTANGLE_SHAPE_2D_H
#define RECTANGLE_SHAPE_2D_H

#include "scene/resources/shape_2d.h"

class RectangleShape2D : public Shape2D {
	GDCLASS(RectangleShape2D, Shape2D);

	Size2 size;
	void _update_shape();

protected:
	static void _bind_methods();
#ifndef DISABLE_DEPRECATED
	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_property) const;
#endif // DISABLE_DEPRECATED

public:
	void set_size(const Size2 &p_size);
	Size2 get_size() const;

	virtual void draw(const RID &p_to_rid, const Color &p_color) override;
	virtual Rect2 get_rect() const override;
	virtual real_t get_enclosing_radius() const override;

	RectangleShape2D();
};

#endif // RECTANGLE_SHAPE_2D_H
