// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "box_shape_3d.h"
#include "servers/physics_server_3d.h"

Vector<Vector3> BoxShape3D::get_debug_mesh_lines() const {
	Vector<Vector3> lines;
	AABB aabb;
	aabb.position = -size / 2;
	aabb.size = size;

	for (int i = 0; i < 12; i++) {
		Vector3 a, b;
		aabb.get_edge(i, a, b);
		lines.push_back(a);
		lines.push_back(b);
	}

	return lines;
}

real_t BoxShape3D::get_enclosing_radius() const {
	return size.length() / 2;
}

void BoxShape3D::_update_shape() {
	PhysicsServer3D::get_singleton()->shape_set_data(get_shape(), size / 2);
	Shape3D::_update_shape();
}

#ifndef DISABLE_DEPRECATED
bool BoxShape3D::_set(const StringName &p_name, const Variant &p_value) {
	if (p_name == "extents") { // Compatibility with Godot 3.x.
		// Convert to `size`, twice as big.
		set_size((Vector3)p_value * 2);
		return true;
	}
	return false;
}

bool BoxShape3D::_get(const StringName &p_name, Variant &r_property) const {
	if (p_name == "extents") { // Compatibility with Godot 3.x.
		// Convert to `extents`, half as big.
		r_property = size / 2;
		return true;
	}
	return false;
}
#endif // DISABLE_DEPRECATED

void BoxShape3D::set_size(const Vector3 &p_size) {
	ERR_FAIL_COND_MSG(p_size.x < 0 || p_size.y < 0 || p_size.z < 0, "BoxShape3D size cannot be negative.");
	size = p_size;
	_update_shape();
	notify_change_to_owners();
}

Vector3 BoxShape3D::get_size() const {
	return size;
}

void BoxShape3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_size", "size"), &BoxShape3D::set_size);
	ClassDB::bind_method(D_METHOD("get_size"), &BoxShape3D::get_size);

	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "size", PROPERTY_HINT_NONE, "suffix:m"), "set_size", "get_size");
}

BoxShape3D::BoxShape3D() :
		Shape3D(PhysicsServer3D::get_singleton()->shape_create(PhysicsServer3D::SHAPE_BOX)) {
	set_size(Vector3(1, 1, 1));
}
