// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef GJK_EPA_H
#define GJK_EPA_H

#include "godot_collision_solver_3d.h"
#include "godot_shape_3d.h"

bool gjk_epa_calculate_penetration(const GodotShape3D *p_shape_A, const Transform3D &p_transform_A, const GodotShape3D *p_shape_B, const Transform3D &p_transform_B, GodotCollisionSolver3D::CallbackResult p_result_callback, void *p_userdata, bool p_swap = false, real_t p_margin_A = 0.0, real_t p_margin_B = 0.0);
bool gjk_epa_calculate_distance(const GodotShape3D *p_shape_A, const Transform3D &p_transform_A, const GodotShape3D *p_shape_B, const Transform3D &p_transform_B, Vector3 &r_result_A, Vector3 &r_result_B);

#endif // GJK_EPA_H
