// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef GODOT_COLLISION_SOLVER_2D_SAT_H
#define GODOT_COLLISION_SOLVER_2D_SAT_H

#include "godot_collision_solver_2d.h"

bool sat_2d_calculate_penetration(const GodotShape2D *p_shape_A, const Transform2D &p_transform_A, const Vector2 &p_motion_A, const GodotShape2D *p_shape_B, const Transform2D &p_transform_B, const Vector2 &p_motion_B, GodotCollisionSolver2D::CallbackResult p_result_callback, void *p_userdata, bool p_swap = false, Vector2 *sep_axis = nullptr, real_t p_margin_A = 0, real_t p_margin_B = 0);

#endif // GODOT_COLLISION_SOLVER_2D_SAT_H
