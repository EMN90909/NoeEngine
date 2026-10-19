// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef SEPARATION_RAY_SHAPE_3D_H
#define SEPARATION_RAY_SHAPE_3D_H

#include "scene/resources/shape_3d.h"

class SeparationRayShape3D : public Shape3D {
	GDCLASS(SeparationRayShape3D, Shape3D);
	float length = 1.0;
	bool slide_on_slope = false;

protected:
	static void _bind_methods();
	virtual void _update_shape() override;

public:
	void set_length(float p_length);
	float get_length() const;

	void set_slide_on_slope(bool p_active);
	bool get_slide_on_slope() const;

	virtual Vector<Vector3> get_debug_mesh_lines() const override;
	virtual real_t get_enclosing_radius() const override;

	SeparationRayShape3D();
};

#endif // SEPARATION_RAY_SHAPE_3D_H
