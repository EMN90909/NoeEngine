// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef SEPARATION_RAY_SHAPE_2D_H
#define SEPARATION_RAY_SHAPE_2D_H

#include "scene/resources/shape_2d.h"

class SeparationRayShape2D : public Shape2D {
	GDCLASS(SeparationRayShape2D, Shape2D);

	real_t length = 20.0;
	bool slide_on_slope = false;

	void _update_shape();

protected:
	static void _bind_methods();

public:
	void set_length(real_t p_length);
	real_t get_length() const;

	void set_slide_on_slope(bool p_active);
	bool get_slide_on_slope() const;

	virtual void draw(const RID &p_to_rid, const Color &p_color) override;
	virtual Rect2 get_rect() const override;
	virtual real_t get_enclosing_radius() const override;

	SeparationRayShape2D();
};

#endif // SEPARATION_RAY_SHAPE_2D_H
