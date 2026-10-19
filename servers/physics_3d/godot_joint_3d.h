// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef GODOT_JOINT_3D_H
#define GODOT_JOINT_3D_H

#include "godot_body_3d.h"
#include "godot_constraint_3d.h"

class GodotJoint3D : public GodotConstraint3D {
protected:
	bool dynamic_A = false;
	bool dynamic_B = false;

public:
	virtual bool setup(real_t p_step) override { return false; }
	virtual bool pre_solve(real_t p_step) override { return true; }
	virtual void solve(real_t p_step) override {}

	void copy_settings_from(GodotJoint3D *p_joint) {
		set_self(p_joint->get_self());
		set_priority(p_joint->get_priority());
		disable_collisions_between_bodies(p_joint->is_disabled_collisions_between_bodies());
	}

	virtual PhysicsServer3D::JointType get_type() const { return PhysicsServer3D::JOINT_TYPE_MAX; }
	_FORCE_INLINE_ GodotJoint3D(GodotBody3D **p_body_ptr = nullptr, int p_body_count = 0) :
			GodotConstraint3D(p_body_ptr, p_body_count) {
	}

	virtual ~GodotJoint3D() {
		for (int i = 0; i < get_body_count(); i++) {
			GodotBody3D *body = get_body_ptr()[i];
			if (body) {
				body->remove_constraint(this);
			}
		}
	}
};

#endif // GODOT_JOINT_3D_H
