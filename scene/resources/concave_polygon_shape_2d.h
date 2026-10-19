// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef CONCAVE_POLYGON_SHAPE_2D_H
#define CONCAVE_POLYGON_SHAPE_2D_H

#include "scene/resources/shape_2d.h"

class ConcavePolygonShape2D : public Shape2D {
	GDCLASS(ConcavePolygonShape2D, Shape2D);

protected:
	static void _bind_methods();

public:
	virtual bool _edit_is_selected_on_click(const Point2 &p_point, double p_tolerance) const override;

	void set_segments(const Vector<Vector2> &p_segments);
	Vector<Vector2> get_segments() const;

	virtual void draw(const RID &p_to_rid, const Color &p_color) override;
	virtual Rect2 get_rect() const override;
	virtual real_t get_enclosing_radius() const override;

	ConcavePolygonShape2D();
};

#endif // CONCAVE_POLYGON_SHAPE_2D_H
