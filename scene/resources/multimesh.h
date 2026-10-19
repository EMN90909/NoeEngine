// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef MULTIMESH_H
#define MULTIMESH_H

#include "scene/resources/mesh.h"
#include "servers/rendering_server.h"

class MultiMesh : public Resource {
	GDCLASS(MultiMesh, Resource);
	RES_BASE_EXTENSION("multimesh");

public:
	enum TransformFormat {
		TRANSFORM_2D = RS::MULTIMESH_TRANSFORM_2D,
		TRANSFORM_3D = RS::MULTIMESH_TRANSFORM_3D
	};

private:
	Ref<Mesh> mesh;
	RID multimesh;
	TransformFormat transform_format = TRANSFORM_2D;
	bool use_colors = false;
	bool use_custom_data = false;
	int instance_count = 0;
	int visible_instance_count = -1;

protected:
	static void _bind_methods();

#ifndef DISABLE_DEPRECATED
	// Kept for compatibility from 3.x to 4.0.
	void _set_transform_array(const Vector<Vector3> &p_array);
	Vector<Vector3> _get_transform_array() const;

	void _set_transform_2d_array(const Vector<Vector2> &p_array);
	Vector<Vector2> _get_transform_2d_array() const;

	void _set_color_array(const Vector<Color> &p_array);
	Vector<Color> _get_color_array() const;

	void _set_custom_data_array(const Vector<Color> &p_array);
	Vector<Color> _get_custom_data_array() const;
#endif
	void set_buffer(const Vector<float> &p_buffer);
	Vector<float> get_buffer() const;

public:
	void set_mesh(const Ref<Mesh> &p_mesh);
	Ref<Mesh> get_mesh() const;

	void set_use_colors(bool p_enable);
	bool is_using_colors() const;

	void set_use_custom_data(bool p_enable);
	bool is_using_custom_data() const;

	void set_transform_format(TransformFormat p_transform_format);
	TransformFormat get_transform_format() const;

	void set_instance_count(int p_count);
	int get_instance_count() const;

	void set_visible_instance_count(int p_count);
	int get_visible_instance_count() const;

	void set_instance_transform(int p_instance, const Transform3D &p_transform);
	void set_instance_transform_2d(int p_instance, const Transform2D &p_transform);
	Transform3D get_instance_transform(int p_instance) const;
	Transform2D get_instance_transform_2d(int p_instance) const;

	void set_instance_color(int p_instance, const Color &p_color);
	Color get_instance_color(int p_instance) const;

	void set_instance_custom_data(int p_instance, const Color &p_custom_data);
	Color get_instance_custom_data(int p_instance) const;

	virtual AABB get_aabb() const;

	virtual RID get_rid() const override;

	MultiMesh();
	~MultiMesh();
};

VARIANT_ENUM_CAST(MultiMesh::TransformFormat);

#endif // MULTIMESH_H
