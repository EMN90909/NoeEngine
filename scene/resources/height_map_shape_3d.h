// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef HEIGHT_MAP_SHAPE_3D_H
#define HEIGHT_MAP_SHAPE_3D_H

#include "scene/resources/shape_3d.h"

class HeightMapShape3D : public Shape3D {
	GDCLASS(HeightMapShape3D, Shape3D);

	int map_width = 2;
	int map_depth = 2;
	Vector<real_t> map_data;
	real_t min_height = 0.0;
	real_t max_height = 0.0;

protected:
	static void _bind_methods();
	virtual void _update_shape() override;

public:
	void set_map_width(int p_new);
	int get_map_width() const;
	void set_map_depth(int p_new);
	int get_map_depth() const;
	void set_map_data(Vector<real_t> p_new);
	Vector<real_t> get_map_data() const;

	virtual Vector<Vector3> get_debug_mesh_lines() const override;
	virtual real_t get_enclosing_radius() const override;

	HeightMapShape3D();
};

#endif // HEIGHT_MAP_SHAPE_3D_H
