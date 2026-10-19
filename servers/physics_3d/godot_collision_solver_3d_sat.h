// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef GODOT_COLLISION_SOLVER_3D_SAT_H
#define GODOT_COLLISION_SOLVER_3D_SAT_H

#include "godot_collision_solver_3d.h"

bool sat_calculate_penetration(const GodotShape3D *p_shape_A, const Transform3D &p_transform_A, const GodotShape3D *p_shape_B, const Transform3D &p_transform_B, GodotCollisionSolver3D::CallbackResult p_result_callback, void *p_userdata, bool p_swap = false, Vector3 *r_prev_axis = nullptr, real_t p_margin_a = 0, real_t p_margin_b = 0);

#endif // GODOT_COLLISION_SOLVER_3D_SAT_H
