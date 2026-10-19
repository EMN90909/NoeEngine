// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "marker_3d.h"

void Marker3D::set_gizmo_extents(real_t p_extents) {
	if (Math::is_equal_approx(gizmo_extents, p_extents)) {
		return;
	}
	gizmo_extents = p_extents;
	update_gizmos();
}

real_t Marker3D::get_gizmo_extents() const {
	return gizmo_extents;
}

void Marker3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_gizmo_extents", "extents"), &Marker3D::set_gizmo_extents);
	ClassDB::bind_method(D_METHOD("get_gizmo_extents"), &Marker3D::get_gizmo_extents);

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "gizmo_extents", PROPERTY_HINT_RANGE, "0,10,0.01,or_greater,suffix:m"), "set_gizmo_extents", "get_gizmo_extents");
}

Marker3D::Marker3D() {
}
