// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef WORLD_BOUNDARY_SHAPE_3D_H
#define WORLD_BOUNDARY_SHAPE_3D_H

#include "scene/resources/shape_3d.h"

class WorldBoundaryShape3D : public Shape3D {
	GDCLASS(WorldBoundaryShape3D, Shape3D);
	Plane plane;

protected:
	static void _bind_methods();
	virtual void _update_shape() override;

public:
	void set_plane(const Plane &p_plane);
	const Plane &get_plane() const;

	virtual Vector<Vector3> get_debug_mesh_lines() const override;
	virtual real_t get_enclosing_radius() const override {
		// Should be infinite?
		return 0;
	}

	WorldBoundaryShape3D();
};

#endif // WORLD_BOUNDARY_SHAPE_3D_H
