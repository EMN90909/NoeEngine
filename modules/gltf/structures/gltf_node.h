// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef GLTF_NODE_H
#define GLTF_NODE_H

#include "../gltf_defines.h"
#include "core/io/resource.h"

class GLTFNode : public Resource {
	GDCLASS(GLTFNode, Resource);
	friend class GLTFDocument;

private:
	// matrices need to be transformed to this
	GLTFNodeIndex parent = -1;
	int height = -1;
	Transform3D xform;
	GLTFMeshIndex mesh = -1;
	GLTFCameraIndex camera = -1;
	GLTFSkinIndex skin = -1;
	GLTFSkeletonIndex skeleton = -1;
	bool joint = false;
	Vector3 position;
	Quaternion rotation;
	Vector3 scale = Vector3(1, 1, 1);
	Vector<int> children;
	GLTFLightIndex light = -1;
	Dictionary additional_data;

protected:
	static void _bind_methods();

public:
	GLTFNodeIndex get_parent();
	void set_parent(GLTFNodeIndex p_parent);

	int get_height();
	void set_height(int p_height);

	Transform3D get_xform();
	void set_xform(Transform3D p_xform);

	GLTFMeshIndex get_mesh();
	void set_mesh(GLTFMeshIndex p_mesh);

	GLTFCameraIndex get_camera();
	void set_camera(GLTFCameraIndex p_camera);

	GLTFSkinIndex get_skin();
	void set_skin(GLTFSkinIndex p_skin);

	GLTFSkeletonIndex get_skeleton();
	void set_skeleton(GLTFSkeletonIndex p_skeleton);

	Vector3 get_position();
	void set_position(Vector3 p_position);

	Quaternion get_rotation();
	void set_rotation(Quaternion p_rotation);

	Vector3 get_scale();
	void set_scale(Vector3 p_scale);

	Vector<int> get_children();
	void set_children(Vector<int> p_children);

	GLTFLightIndex get_light();
	void set_light(GLTFLightIndex p_light);

	Variant get_additional_data(const StringName &p_extension_name);
	void set_additional_data(const StringName &p_extension_name, Variant p_additional_data);
};

#endif // GLTF_NODE_H
