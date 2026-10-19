// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef COLLISION_SHAPE_3D_H
#define COLLISION_SHAPE_3D_H

#include "scene/3d/node_3d.h"
#include "scene/resources/shape_3d.h"

class CollisionObject3D;
class CollisionShape3D : public Node3D {
	GDCLASS(CollisionShape3D, Node3D);

	Ref<Shape3D> shape;

	uint32_t owner_id = 0;
	CollisionObject3D *parent = nullptr;

	void resource_changed(Ref<Resource> res);
	bool disabled = false;

protected:
	void _update_in_shape_owner(bool p_xform_only = false);

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	void make_convex_from_siblings();

	void set_shape(const Ref<Shape3D> &p_shape);
	Ref<Shape3D> get_shape() const;

	void set_disabled(bool p_disabled);
	bool is_disabled() const;

	PackedStringArray get_configuration_warnings() const override;

	CollisionShape3D();
	~CollisionShape3D();
};

#endif // COLLISION_SHAPE_3D_H
