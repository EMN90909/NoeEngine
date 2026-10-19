// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef SEGMENT_SHAPE_2D_H
#define SEGMENT_SHAPE_2D_H

#include "scene/resources/shape_2d.h"

class SegmentShape2D : public Shape2D {
	GDCLASS(SegmentShape2D, Shape2D);

	Vector2 a;
	Vector2 b;

	void _update_shape();

protected:
	static void _bind_methods();

public:
	virtual bool _edit_is_selected_on_click(const Point2 &p_point, double p_tolerance) const override;

	void set_a(const Vector2 &p_a);
	void set_b(const Vector2 &p_b);

	Vector2 get_a() const;
	Vector2 get_b() const;

	virtual void draw(const RID &p_to_rid, const Color &p_color) override;
	virtual Rect2 get_rect() const override;
	virtual real_t get_enclosing_radius() const override;

	SegmentShape2D();
};

#endif // SEGMENT_SHAPE_2D_H
