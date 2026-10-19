// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef GODOT_AREA_PAIR_2D_H
#define GODOT_AREA_PAIR_2D_H

#include "godot_area_2d.h"
#include "godot_body_2d.h"
#include "godot_constraint_2d.h"

class GodotAreaPair2D : public GodotConstraint2D {
	GodotBody2D *body = nullptr;
	GodotArea2D *area = nullptr;
	int body_shape = 0;
	int area_shape = 0;
	bool colliding = false;
	bool has_space_override = false;
	bool process_collision = false;

public:
	virtual bool setup(real_t p_step) override;
	virtual bool pre_solve(real_t p_step) override;
	virtual void solve(real_t p_step) override;

	GodotAreaPair2D(GodotBody2D *p_body, int p_body_shape, GodotArea2D *p_area, int p_area_shape);
	~GodotAreaPair2D();
};

class GodotArea2Pair2D : public GodotConstraint2D {
	GodotArea2D *area_a = nullptr;
	GodotArea2D *area_b = nullptr;
	int shape_a = 0;
	int shape_b = 0;
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

	GodotArea2Pair2D(GodotArea2D *p_area_a, int p_shape_a, GodotArea2D *p_area_b, int p_shape_b);
	~GodotArea2Pair2D();
};

#endif // GODOT_AREA_PAIR_2D_H
