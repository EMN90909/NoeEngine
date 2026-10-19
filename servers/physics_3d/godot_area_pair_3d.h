// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef GODOT_AREA_PAIR_3D_H
#define GODOT_AREA_PAIR_3D_H

#include "godot_area_3d.h"
#include "godot_body_3d.h"
#include "godot_constraint_3d.h"
#include "godot_soft_body_3d.h"

class GodotAreaPair3D : public GodotConstraint3D {
	GodotBody3D *body = nullptr;
	GodotArea3D *area = nullptr;
	int body_shape;
	int area_shape;
	bool colliding = false;
	bool process_collision = false;
	bool has_space_override = false;

public:
	virtual bool setup(real_t p_step) override;
	virtual bool pre_solve(real_t p_step) override;
	virtual void solve(real_t p_step) override;

	GodotAreaPair3D(GodotBody3D *p_body, int p_body_shape, GodotArea3D *p_area, int p_area_shape);
	~GodotAreaPair3D();
};

class GodotArea2Pair3D : public GodotConstraint3D {
	GodotArea3D *area_a = nullptr;
	GodotArea3D *area_b = nullptr;
	int shape_a;
	int shape_b;
	bool colliding_a = false;
	bool colliding_b = false;
	bool process_collision_a = false;
	bool process_collision_b = false;
	bool area_a_monitorable;
	bool area_b_monitorable;

public:
	virtual bool setup(real_t p_step) override;
	virtual bool pre_solve(real_t p_step) override;
	virtual void solve(real_t p_step) override;

	GodotArea2Pair3D(GodotArea3D *p_area_a, int p_shape_a, GodotArea3D *p_area_b, int p_shape_b);
	~GodotArea2Pair3D();
};

class GodotAreaSoftBodyPair3D : public GodotConstraint3D {
	GodotSoftBody3D *soft_body = nullptr;
	GodotArea3D *area = nullptr;
	int soft_body_shape;
	int area_shape;
	bool colliding = false;
	bool process_collision = false;
	bool has_space_override = false;

public:
	virtual bool setup(real_t p_step) override;
	virtual bool pre_solve(real_t p_step) override;
	virtual void solve(real_t p_step) override;

	GodotAreaSoftBodyPair3D(GodotSoftBody3D *p_sof_body, int p_soft_body_shape, GodotArea3D *p_area, int p_area_shape);
	~GodotAreaSoftBodyPair3D();
};

#endif // GODOT_AREA_PAIR_3D_H
