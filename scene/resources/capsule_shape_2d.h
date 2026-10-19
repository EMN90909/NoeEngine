// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef CAPSULE_SHAPE_2D_H
#define CAPSULE_SHAPE_2D_H

#include "scene/resources/shape_2d.h"

class CapsuleShape2D : public Shape2D {
	GDCLASS(CapsuleShape2D, Shape2D);

	real_t height = 30.0;
	real_t radius = 10.0;

	void _update_shape();
	Vector<Vector2> _get_points() const;

protected:
	static void _bind_methods();

public:
	virtual bool _edit_is_selected_on_click(const Point2 &p_point, double p_tolerance) const override;

	void set_height(real_t p_height);
	real_t get_height() const;

	void set_radius(real_t p_radius);
	real_t get_radius() const;

	virtual void draw(const RID &p_to_rid, const Color &p_color) override;
	virtual Rect2 get_rect() const override;
	virtual real_t get_enclosing_radius() const override;

	CapsuleShape2D();
};

#endif // CAPSULE_SHAPE_2D_H
