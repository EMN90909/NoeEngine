// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef SPHERE_SHAPE_3D_H
#define SPHERE_SHAPE_3D_H

#include "scene/resources/shape_3d.h"

class SphereShape3D : public Shape3D {
	GDCLASS(SphereShape3D, Shape3D);
	float radius = 0.5f;

protected:
	static void _bind_methods();

	virtual void _update_shape() override;

public:
	void set_radius(float p_radius);
	float get_radius() const;

	virtual Vector<Vector3> get_debug_mesh_lines() const override;
	virtual real_t get_enclosing_radius() const override;

	SphereShape3D();
};

#endif // SPHERE_SHAPE_3D_H
