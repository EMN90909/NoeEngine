// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef PATH_3D_H
#define PATH_3D_H

#include "scene/3d/node_3d.h"
#include "scene/resources/curve.h"

class Path3D : public Node3D {
	GDCLASS(Path3D, Node3D);

	Ref<Curve3D> curve;

	void _curve_changed();

	RID debug_instance;
	Ref<ArrayMesh> debug_mesh;

private:
	void _update_debug_mesh();

protected:
	void _notification(int p_what);

	static void _bind_methods();

public:
	void set_curve(const Ref<Curve3D> &p_curve);
	Ref<Curve3D> get_curve() const;

	Path3D();
	~Path3D();
};

class PathFollow3D : public Node3D {
	GDCLASS(PathFollow3D, Node3D);

public:
	enum RotationMode {
		ROTATION_NONE,
		ROTATION_Y,
		ROTATION_XY,
		ROTATION_XYZ,
		ROTATION_ORIENTED
	};

	static Transform3D correct_posture(Transform3D p_transform, PathFollow3D::RotationMode p_rotation_mode);

private:
	Path3D *path = nullptr;
	real_t progress = 0.0;
	real_t h_offset = 0.0;
	real_t v_offset = 0.0;
	bool cubic = true;
	bool loop = true;
	bool tilt_enabled = true;
	RotationMode rotation_mode = ROTATION_XYZ;

	void _update_transform(bool p_update_xyz_rot = true);

protected:
	void _validate_property(PropertyInfo &p_property) const;

	void _notification(int p_what);
	static void _bind_methods();

public:
	void set_progress(real_t p_progress);
	real_t get_progress() const;

	void set_h_offset(real_t p_h_offset);
	real_t get_h_offset() const;

	void set_v_offset(real_t p_v_offset);
	real_t get_v_offset() const;

	void set_progress_ratio(real_t p_ratio);
	real_t get_progress_ratio() const;

	void set_loop(bool p_loop);
	bool has_loop() const;

	void set_tilt_enabled(bool p_enable);
	bool is_tilt_enabled() const;

	void set_rotation_mode(RotationMode p_rotation_mode);
	RotationMode get_rotation_mode() const;

	void set_cubic_interpolation(bool p_enable);
	bool get_cubic_interpolation() const;

	PackedStringArray get_configuration_warnings() const override;

	PathFollow3D() {}
};

VARIANT_ENUM_CAST(PathFollow3D::RotationMode);

#endif // PATH_3D_H
