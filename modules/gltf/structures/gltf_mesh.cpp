// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "gltf_mesh.h"

#include "scene/resources/importer_mesh.h"

void GLTFMesh::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_mesh"), &GLTFMesh::get_mesh);
	ClassDB::bind_method(D_METHOD("set_mesh", "mesh"), &GLTFMesh::set_mesh);
	ClassDB::bind_method(D_METHOD("get_blend_weights"), &GLTFMesh::get_blend_weights);
	ClassDB::bind_method(D_METHOD("set_blend_weights", "blend_weights"), &GLTFMesh::set_blend_weights);
	ClassDB::bind_method(D_METHOD("get_instance_materials"), &GLTFMesh::get_instance_materials);
	ClassDB::bind_method(D_METHOD("set_instance_materials", "instance_materials"), &GLTFMesh::set_instance_materials);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "mesh"), "set_mesh", "get_mesh");
	ADD_PROPERTY(PropertyInfo(Variant::PACKED_FLOAT32_ARRAY, "blend_weights"), "set_blend_weights", "get_blend_weights"); // Vector<float>
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "instance_materials"), "set_instance_materials", "get_instance_materials");
}

Ref<ImporterMesh> GLTFMesh::get_mesh() {
	return mesh;
}

void GLTFMesh::set_mesh(Ref<ImporterMesh> p_mesh) {
	mesh = p_mesh;
}

TypedArray<Material> GLTFMesh::get_instance_materials() {
	return instance_materials;
}

void GLTFMesh::set_instance_materials(TypedArray<Material> p_instance_materials) {
	instance_materials = p_instance_materials;
}

Vector<float> GLTFMesh::get_blend_weights() {
	return blend_weights;
}

void GLTFMesh::set_blend_weights(Vector<float> p_blend_weights) {
	blend_weights = p_blend_weights;
}
