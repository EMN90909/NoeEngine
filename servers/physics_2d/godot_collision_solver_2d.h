// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef GODOT_COLLISION_SOLVER_2D_H
#define GODOT_COLLISION_SOLVER_2D_H

#include "godot_shape_2d.h"

class GodotCollisionSolver2D {
public:
	typedef void (*CallbackResult)(const Vector2 &p_point_A, const Vector2 &p_point_B, void *p_userdata);

private:
	static bool solve_static_world_boundary(const GodotShape2D *p_shape_A, const Transform2D &p_transform_A, const GodotShape2D *p_shape_B, const Transform2D &p_transform_B, const Vector2 &p_motion_B, CallbackResult p_result_callback, void *p_userdata, bool p_swap_result, real_t p_margin = 0);
	static bool concave_callback(void *p_userdata, GodotShape2D *p_convex);
	static bool solve_concave(const GodotShape2D *p_shape_A, const Transform2D &p_transform_A, const Vector2 &p_motion_A, const GodotShape2D *p_shape_B, const Transform2D &p_transform_B, const Vector2 &p_motion_B, CallbackResult p_result_callback, void *p_userdata, bool p_swap_result, Vector2 *r_sep_axis = nullptr, real_t p_margin_A = 0, real_t p_margin_B = 0);
	static bool solve_separation_ray(const GodotShape2D *p_shape_A, const Vector2 &p_motion_A, const Transform2D &p_transform_A, const GodotShape2D *p_shape_B, const Transform2D &p_transform_B, CallbackResult p_result_callback, void *p_userdata, bool p_swap_result, Vector2 *r_sep_axis = nullptr, real_t p_margin = 0);

public:
	static bool solve(const GodotShape2D *p_shape_A, const Transform2D &p_transform_A, const Vector2 &p_motion_A, const GodotShape2D *p_shape_B, const Transform2D &p_transform_B, const Vector2 &p_motion_B, CallbackResult p_result_callback, void *p_userdata, Vector2 *r_sep_axis = nullptr, real_t p_margin_A = 0, real_t p_margin_B = 0);
};

#endif // GODOT_COLLISION_SOLVER_2D_H
