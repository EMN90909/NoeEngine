// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef WORLD_BOUNDARY_SHAPE_2D_H
#define WORLD_BOUNDARY_SHAPE_2D_H

#include "scene/resources/shape_2d.h"

class WorldBoundaryShape2D : public Shape2D {
	GDCLASS(WorldBoundaryShape2D, Shape2D);

	// WorldBoundaryShape2D is often used for one-way platforms, where the normal pointing up makes sense.
	Vector2 normal = Vector2(0, -1);
	real_t distance = 0.0;

	void _update_shape();

protected:
	static void _bind_methods();

public:
	virtual bool _edit_is_selected_on_click(const Point2 &p_point, double p_tolerance) const override;

	void set_normal(const Vector2 &p_normal);
	void set_distance(real_t p_distance);

	Vector2 get_normal() const;
	real_t get_distance() const;

	virtual void draw(const RID &p_to_rid, const Color &p_color) override;
	virtual Rect2 get_rect() const override;
	virtual real_t get_enclosing_radius() const override;

	WorldBoundaryShape2D();
};

#endif // WORLD_BOUNDARY_SHAPE_2D_H
