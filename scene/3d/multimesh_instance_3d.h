// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef MULTIMESH_INSTANCE_3D_H
#define MULTIMESH_INSTANCE_3D_H

#include "scene/3d/visual_instance_3d.h"
#include "scene/resources/multimesh.h"

class MultiMeshInstance3D : public GeometryInstance3D {
	GDCLASS(MultiMeshInstance3D, GeometryInstance3D);

	Ref<MultiMesh> multimesh;

protected:
	static void _bind_methods();
	// bind helpers

public:
	void set_multimesh(const Ref<MultiMesh> &p_multimesh);
	Ref<MultiMesh> get_multimesh() const;

	virtual AABB get_aabb() const override;

	MultiMeshInstance3D();
	~MultiMeshInstance3D();
};

#endif // MULTIMESH_INSTANCE_3D_H
