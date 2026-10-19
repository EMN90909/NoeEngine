// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "convex_polygon_shape_3d.h"
#include "core/math/convex_hull.h"
#include "servers/physics_server_3d.h"

Vector<Vector3> ConvexPolygonShape3D::get_debug_mesh_lines() const {
	Vector<Vector3> poly_points = get_points();

	if (poly_points.size() > 3) {
		Vector<Vector3> varr = Variant(poly_points);
		Geometry3D::MeshData md;
		Error err = ConvexHullComputer::convex_hull(varr, md);
		if (err == OK) {
			Vector<Vector3> lines;
			lines.resize(md.edges.size() * 2);
			for (uint32_t i = 0; i < md.edges.size(); i++) {
				lines.write[i * 2 + 0] = md.vertices[md.edges[i].vertex_a];
				lines.write[i * 2 + 1] = md.vertices[md.edges[i].vertex_b];
			}
			return lines;
		}
	}

	return Vector<Vector3>();
}

real_t ConvexPolygonShape3D::get_enclosing_radius() const {
	Vector<Vector3> data = get_points();
	const Vector3 *read = data.ptr();
	real_t r = 0.0;
	for (int i(0); i < data.size(); i++) {
		r = MAX(read[i].length_squared(), r);
	}
	return Math::sqrt(r);
}

void ConvexPolygonShape3D::_update_shape() {
	PhysicsServer3D::get_singleton()->shape_set_data(get_shape(), points);
	Shape3D::_update_shape();
}

void ConvexPolygonShape3D::set_points(const Vector<Vector3> &p_points) {
	points = p_points;
	_update_shape();
	notify_change_to_owners();
}

Vector<Vector3> ConvexPolygonShape3D::get_points() const {
	return points;
}

void ConvexPolygonShape3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_points", "points"), &ConvexPolygonShape3D::set_points);
	ClassDB::bind_method(D_METHOD("get_points"), &ConvexPolygonShape3D::get_points);

	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "points"), "set_points", "get_points");
}

ConvexPolygonShape3D::ConvexPolygonShape3D() :
		Shape3D(PhysicsServer3D::get_singleton()->shape_create(PhysicsServer3D::SHAPE_CONVEX_POLYGON)) {
}
