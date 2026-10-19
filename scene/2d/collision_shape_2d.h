// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef COLLISION_SHAPE_2D_H
#define COLLISION_SHAPE_2D_H

#include "scene/2d/node_2d.h"
#include "scene/resources/shape_2d.h"

class CollisionObject2D;

class CollisionShape2D : public Node2D {
	GDCLASS(CollisionShape2D, Node2D);
	Ref<Shape2D> shape;
	Rect2 rect = Rect2(-Point2(10, 10), Point2(20, 20));
	uint32_t owner_id = 0;
	CollisionObject2D *parent = nullptr;
	bool disabled = false;
	bool one_way_collision = false;
	real_t one_way_collision_margin = 1.0;
	Color debug_color;

	void _shape_changed();
	void _update_in_shape_owner(bool p_xform_only = false);
	Color _get_default_debug_color() const;

protected:
	void _notification(int p_what);
	bool _property_can_revert(const StringName &p_name) const;
	bool _property_get_revert(const StringName &p_name, Variant &r_property) const;
	void _validate_property(PropertyInfo &p_property) const;
	static void _bind_methods();

public:
#ifdef TOOLS_ENABLED
	virtual bool _edit_is_selected_on_click(const Point2 &p_point, double p_tolerance) const override;
#else
	virtual bool _edit_is_selected_on_click(const Point2 &p_point, double p_tolerance) const;
#endif // TOOLS_ENABLED

	void set_shape(const Ref<Shape2D> &p_shape);
	Ref<Shape2D> get_shape() const;

	void set_disabled(bool p_disabled);
	bool is_disabled() const;

	void set_one_way_collision(bool p_enable);
	bool is_one_way_collision_enabled() const;

	void set_one_way_collision_margin(real_t p_margin);
	real_t get_one_way_collision_margin() const;

	void set_debug_color(const Color &p_color);
	Color get_debug_color() const;

	PackedStringArray get_configuration_warnings() const override;

	CollisionShape2D();
};

#endif // COLLISION_SHAPE_2D_H
