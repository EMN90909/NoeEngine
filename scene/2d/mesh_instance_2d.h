// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef MESH_INSTANCE_2D_H
#define MESH_INSTANCE_2D_H

#include "scene/2d/node_2d.h"

class MeshInstance2D : public Node2D {
	GDCLASS(MeshInstance2D, Node2D);

	Ref<Mesh> mesh;

	Ref<Texture2D> texture;

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
#ifdef TOOLS_ENABLED
	virtual Rect2 _edit_get_rect() const override;
	virtual bool _edit_use_rect() const override;
#endif

	void set_mesh(const Ref<Mesh> &p_mesh);
	Ref<Mesh> get_mesh() const;

	void set_texture(const Ref<Texture2D> &p_texture);
	Ref<Texture2D> get_texture() const;

	MeshInstance2D();
};

#endif // MESH_INSTANCE_2D_H
