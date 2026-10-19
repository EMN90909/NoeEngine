// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef GLTF_MESH_H
#define GLTF_MESH_H

#include "../gltf_defines.h"
#include "scene/resources/importer_mesh.h"

class GLTFMesh : public Resource {
	GDCLASS(GLTFMesh, Resource);

private:
	Ref<ImporterMesh> mesh;
	Vector<float> blend_weights;
	TypedArray<Material> instance_materials;

protected:
	static void _bind_methods();

public:
	Ref<ImporterMesh> get_mesh();
	void set_mesh(Ref<ImporterMesh> p_mesh);
	Vector<float> get_blend_weights();
	void set_blend_weights(Vector<float> p_blend_weights);
	TypedArray<Material> get_instance_materials();
	void set_instance_materials(TypedArray<Material> p_instance_materials);
};

#endif // GLTF_MESH_H
