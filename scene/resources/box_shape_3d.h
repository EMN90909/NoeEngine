// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef BOX_SHAPE_3D_H
#define BOX_SHAPE_3D_H

#include "scene/resources/shape_3d.h"

class BoxShape3D : public Shape3D {
	GDCLASS(BoxShape3D, Shape3D);
	Vector3 size;

protected:
	static void _bind_methods();
#ifndef DISABLE_DEPRECATED
	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_property) const;
#endif // DISABLE_DEPRECATED

	virtual void _update_shape() override;

public:
	void set_size(const Vector3 &p_size);
	Vector3 get_size() const;

	virtual Vector<Vector3> get_debug_mesh_lines() const override;
	virtual real_t get_enclosing_radius() const override;

	BoxShape3D();
};

#endif // BOX_SHAPE_3D_H
