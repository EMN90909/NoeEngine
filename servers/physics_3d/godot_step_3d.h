// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef GODOT_STEP_3D_H
#define GODOT_STEP_3D_H

#include "godot_space_3d.h"

#include "core/templates/local_vector.h"

class GodotStep3D {
	uint64_t _step = 1;

	int iterations = 0;
	real_t delta = 0.0;

	LocalVector<LocalVector<GodotBody3D *>> body_islands;
	LocalVector<LocalVector<GodotConstraint3D *>> constraint_islands;
	LocalVector<GodotConstraint3D *> all_constraints;

	void _populate_island(GodotBody3D *p_body, LocalVector<GodotBody3D *> &p_body_island, LocalVector<GodotConstraint3D *> &p_constraint_island);
	void _populate_island_soft_body(GodotSoftBody3D *p_soft_body, LocalVector<GodotBody3D *> &p_body_island, LocalVector<GodotConstraint3D *> &p_constraint_island);
	void _setup_constraint(uint32_t p_constraint_index, void *p_userdata = nullptr);
	void _pre_solve_island(LocalVector<GodotConstraint3D *> &p_constraint_island) const;
	void _solve_island(uint32_t p_island_index, void *p_userdata = nullptr);
	void _check_suspend(const LocalVector<GodotBody3D *> &p_body_island) const;

public:
	void step(GodotSpace3D *p_space, real_t p_delta);
	GodotStep3D();
	~GodotStep3D();
};

#endif // GODOT_STEP_3D_H
