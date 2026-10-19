// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef SPRING_ARM_3D_H
#define SPRING_ARM_3D_H

#include "scene/3d/node_3d.h"

class SpringArm3D : public Node3D {
	GDCLASS(SpringArm3D, Node3D);

	Ref<Shape3D> shape;
	HashSet<RID> excluded_objects;
	real_t spring_length = 1.0;
	real_t current_spring_length = 0.0;
	bool keep_child_basis = false;
	uint32_t mask = 1;
	real_t margin = 0.01;

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	void set_length(real_t p_length);
	real_t get_length() const;
	void set_shape(Ref<Shape3D> p_shape);
	Ref<Shape3D> get_shape() const;
	void set_mask(uint32_t p_mask);
	uint32_t get_mask();
	void add_excluded_object(RID p_rid);
	bool remove_excluded_object(RID p_rid);
	void clear_excluded_objects();
	real_t get_hit_length();
	void set_margin(real_t p_margin);
	real_t get_margin();

	SpringArm3D() {}

private:
	void process_spring();
};

#endif // SPRING_ARM_3D_H
