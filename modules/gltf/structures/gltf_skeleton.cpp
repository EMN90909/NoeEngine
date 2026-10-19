// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "gltf_skeleton.h"

#include "../gltf_template_convert.h"
#include "scene/3d/bone_attachment_3d.h"

void GLTFSkeleton::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_joints"), &GLTFSkeleton::get_joints);
	ClassDB::bind_method(D_METHOD("set_joints", "joints"), &GLTFSkeleton::set_joints);
	ClassDB::bind_method(D_METHOD("get_roots"), &GLTFSkeleton::get_roots);
	ClassDB::bind_method(D_METHOD("set_roots", "roots"), &GLTFSkeleton::set_roots);
	ClassDB::bind_method(D_METHOD("get_godot_skeleton"), &GLTFSkeleton::get_godot_skeleton);
	ClassDB::bind_method(D_METHOD("get_unique_names"), &GLTFSkeleton::get_unique_names);
	ClassDB::bind_method(D_METHOD("set_unique_names", "unique_names"), &GLTFSkeleton::set_unique_names);
	ClassDB::bind_method(D_METHOD("get_godot_bone_node"), &GLTFSkeleton::get_godot_bone_node);
	ClassDB::bind_method(D_METHOD("set_godot_bone_node", "godot_bone_node"), &GLTFSkeleton::set_godot_bone_node);
	ClassDB::bind_method(D_METHOD("get_bone_attachment_count"), &GLTFSkeleton::get_bone_attachment_count);
	ClassDB::bind_method(D_METHOD("get_bone_attachment", "idx"), &GLTFSkeleton::get_bone_attachment);

	ADD_PROPERTY(PropertyInfo(Variant::PACKED_INT32_ARRAY, "joints"), "set_joints", "get_joints"); // Vector<GLTFNodeIndex>
	ADD_PROPERTY(PropertyInfo(Variant::PACKED_INT32_ARRAY, "roots"), "set_roots", "get_roots"); // Vector<GLTFNodeIndex>
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "unique_names", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_STORAGE | PROPERTY_USAGE_INTERNAL | PROPERTY_USAGE_EDITOR), "set_unique_names", "get_unique_names"); // Set<String>
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "godot_bone_node", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_STORAGE | PROPERTY_USAGE_INTERNAL | PROPERTY_USAGE_EDITOR), "set_godot_bone_node", "get_godot_bone_node"); // RBMap<int32_t,
}

Vector<GLTFNodeIndex> GLTFSkeleton::get_joints() {
	return joints;
}

void GLTFSkeleton::set_joints(Vector<GLTFNodeIndex> p_joints) {
	joints = p_joints;
}

Vector<GLTFNodeIndex> GLTFSkeleton::get_roots() {
	return roots;
}

void GLTFSkeleton::set_roots(Vector<GLTFNodeIndex> p_roots) {
	roots = p_roots;
}

Skeleton3D *GLTFSkeleton::get_godot_skeleton() {
	return godot_skeleton;
}

TypedArray<String> GLTFSkeleton::get_unique_names() {
	return GLTFTemplateConvert::to_array(unique_names);
}

void GLTFSkeleton::set_unique_names(TypedArray<String> p_unique_names) {
	GLTFTemplateConvert::set_from_array(unique_names, p_unique_names);
}

Dictionary GLTFSkeleton::get_godot_bone_node() {
	return GLTFTemplateConvert::to_dict(godot_bone_node);
}

void GLTFSkeleton::set_godot_bone_node(Dictionary p_indict) {
	GLTFTemplateConvert::set_from_dict(godot_bone_node, p_indict);
}

BoneAttachment3D *GLTFSkeleton::get_bone_attachment(int idx) {
	ERR_FAIL_INDEX_V(idx, bone_attachments.size(), nullptr);
	return bone_attachments[idx];
}

int32_t GLTFSkeleton::get_bone_attachment_count() {
	return bone_attachments.size();
}
