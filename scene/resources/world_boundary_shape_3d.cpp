// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "world_boundary_shape_3d.h"

#include "servers/physics_server_3d.h"

Vector<Vector3> WorldBoundaryShape3D::get_debug_mesh_lines() const {
	Plane p = get_plane();

	Vector3 n1 = p.get_any_perpendicular_normal();
	Vector3 n2 = p.normal.cross(n1).normalized();

	Vector3 pface[4] = {
		p.normal * p.d + n1 * 10.0 + n2 * 10.0,
		p.normal * p.d + n1 * 10.0 + n2 * -10.0,
		p.normal * p.d + n1 * -10.0 + n2 * -10.0,
		p.normal * p.d + n1 * -10.0 + n2 * 10.0,
	};

	Vector<Vector3> points = {
		pface[0],
		pface[1],
		pface[1],
		pface[2],
		pface[2],
		pface[3],
		pface[3],
		pface[0],
		p.normal * p.d,
		p.normal * p.d + p.normal * 3
	};

	return points;
}

void WorldBoundaryShape3D::_update_shape() {
	PhysicsServer3D::get_singleton()->shape_set_data(get_shape(), plane);
	Shape3D::_update_shape();
}

void WorldBoundaryShape3D::set_plane(const Plane &p_plane) {
	plane = p_plane;
	_update_shape();
	notify_change_to_owners();
}

const Plane &WorldBoundaryShape3D::get_plane() const {
	return plane;
}

void WorldBoundaryShape3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_plane", "plane"), &WorldBoundaryShape3D::set_plane);
	ClassDB::bind_method(D_METHOD("get_plane"), &WorldBoundaryShape3D::get_plane);

	ADD_PROPERTY(PropertyInfo(Variant::PLANE, "plane", PROPERTY_HINT_NONE, "suffix:m"), "set_plane", "get_plane");
}

WorldBoundaryShape3D::WorldBoundaryShape3D() :
		Shape3D(PhysicsServer3D::get_singleton()->shape_create(PhysicsServer3D::SHAPE_WORLD_BOUNDARY)) {
	set_plane(Plane(0, 1, 0, 0));
}
