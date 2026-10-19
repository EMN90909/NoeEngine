// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef GLTF_SKELETON_H
#define GLTF_SKELETON_H

#include "../gltf_defines.h"
#include "core/io/resource.h"

class GLTFSkeleton : public Resource {
	GDCLASS(GLTFSkeleton, Resource);
	friend class GLTFDocument;

private:
	// The *synthesized* skeletons joints
	Vector<GLTFNodeIndex> joints;

	// The roots of the skeleton. If there are multiple, each root must have the
	// same parent (ie roots are siblings)
	Vector<GLTFNodeIndex> roots;

	// The created Skeleton3D for the scene
	Skeleton3D *godot_skeleton = nullptr;

	// Set of unique bone names for the skeleton
	HashSet<String> unique_names;

	HashMap<int32_t, GLTFNodeIndex> godot_bone_node;

	Vector<BoneAttachment3D *> bone_attachments;

protected:
	static void _bind_methods();

public:
	Vector<GLTFNodeIndex> get_joints();
	void set_joints(Vector<GLTFNodeIndex> p_joints);

	Vector<GLTFNodeIndex> get_roots();
	void set_roots(Vector<GLTFNodeIndex> p_roots);

	Skeleton3D *get_godot_skeleton();

	// Skeleton *get_godot_skeleton() {
	// 	return this->godot_skeleton;
	// }
	// void set_godot_skeleton(Skeleton p_*godot_skeleton) {
	// 	this->godot_skeleton = p_godot_skeleton;
	// }

	TypedArray<String> get_unique_names();
	void set_unique_names(TypedArray<String> p_unique_names);

	//RBMap<int32_t, GLTFNodeIndex> get_godot_bone_node() {
	//	return this->godot_bone_node;
	//}
	//void set_godot_bone_node(RBMap<int32_t, GLTFNodeIndex> p_godot_bone_node) {
	//	this->godot_bone_node = p_godot_bone_node;
	//}
	Dictionary get_godot_bone_node();
	void set_godot_bone_node(Dictionary p_indict);

	//Dictionary get_godot_bone_node() {
	//	return VariantConversion::to_dict(this->godot_bone_node);
	//}
	//void set_godot_bone_node(Dictionary p_indict) {
	//	VariantConversion::set_from_dict(this->godot_bone_node, p_indict);
	//}

	BoneAttachment3D *get_bone_attachment(int idx);

	int32_t get_bone_attachment_count();
};

#endif // GLTF_SKELETON_H
