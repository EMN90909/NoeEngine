// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef COLLISION_POLYGON_3D_H
#define COLLISION_POLYGON_3D_H

#include "scene/3d/node_3d.h"
#include "scene/resources/shape_3d.h"

class CollisionObject3D;
class CollisionPolygon3D : public Node3D {
	GDCLASS(CollisionPolygon3D, Node3D);
	real_t margin = 0.04;

protected:
	real_t depth = 1.0;
	AABB aabb = AABB(Vector3(-1, -1, -1), Vector3(2, 2, 2));
	Vector<Point2> polygon;

	uint32_t owner_id = 0;
	CollisionObject3D *parent = nullptr;

	bool disabled = false;

	void _build_polygon();

	void _update_in_shape_owner(bool p_xform_only = false);

	bool _is_editable_3d_polygon() const;

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	void set_depth(real_t p_depth);
	real_t get_depth() const;

	void set_polygon(const Vector<Point2> &p_polygon);
	Vector<Point2> get_polygon() const;

	void set_disabled(bool p_disabled);
	bool is_disabled() const;

	virtual AABB get_item_rect() const;

	real_t get_margin() const;
	void set_margin(real_t p_margin);

	PackedStringArray get_configuration_warnings() const override;

	CollisionPolygon3D();
};

#endif // COLLISION_POLYGON_3D_H
