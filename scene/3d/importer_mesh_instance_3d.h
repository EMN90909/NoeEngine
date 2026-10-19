// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef IMPORTER_MESH_INSTANCE_3D_H
#define IMPORTER_MESH_INSTANCE_3D_H

#include "scene/3d/node_3d.h"
#include "scene/resources/immediate_mesh.h"
#include "scene/resources/skin.h"

class ImporterMesh;

class ImporterMeshInstance3D : public Node3D {
	GDCLASS(ImporterMeshInstance3D, Node3D)

	Ref<ImporterMesh> mesh;
	Ref<Skin> skin;
	NodePath skeleton_path;
	Vector<Ref<Material>> surface_materials;

protected:
	static void _bind_methods();

public:
	void set_mesh(const Ref<ImporterMesh> &p_mesh);
	Ref<ImporterMesh> get_mesh() const;

	void set_skin(const Ref<Skin> &p_skin);
	Ref<Skin> get_skin() const;

	void set_surface_material(int p_idx, const Ref<Material> &p_material);
	Ref<Material> get_surface_material(int p_idx) const;

	void set_skeleton_path(const NodePath &p_path);
	NodePath get_skeleton_path() const;
};

#endif // IMPORTER_MESH_INSTANCE_3D_H
