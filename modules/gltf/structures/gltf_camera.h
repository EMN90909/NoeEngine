// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef GLTF_CAMERA_H
#define GLTF_CAMERA_H

#include "core/io/resource.h"

class Camera3D;

// Reference and test file:
// https://github.com/KhronosGroup/glTF-Tutorials/blob/master/gltfTutorial/gltfTutorial_015_SimpleCameras.md

class GLTFCamera : public Resource {
	GDCLASS(GLTFCamera, Resource);

private:
	// GLTF has no default camera values, they should always be specified in
	// the GLTF file. Here we default to Godot's default camera settings.
	bool perspective = true;
	real_t fov = Math::deg_to_rad(75.0);
	real_t size_mag = 0.5;
	real_t depth_far = 4000.0;
	real_t depth_near = 0.05;

protected:
	static void _bind_methods();

public:
	bool get_perspective() const { return perspective; }
	void set_perspective(bool p_val) { perspective = p_val; }
	real_t get_fov() const { return fov; }
	void set_fov(real_t p_val) { fov = p_val; }
	real_t get_size_mag() const { return size_mag; }
	void set_size_mag(real_t p_val) { size_mag = p_val; }
	real_t get_depth_far() const { return depth_far; }
	void set_depth_far(real_t p_val) { depth_far = p_val; }
	real_t get_depth_near() const { return depth_near; }
	void set_depth_near(real_t p_val) { depth_near = p_val; }

	static Ref<GLTFCamera> from_node(const Camera3D *p_camera);
	Camera3D *to_node() const;

	static Ref<GLTFCamera> from_dictionary(const Dictionary p_dictionary);
	Dictionary to_dictionary() const;
};

#endif // GLTF_CAMERA_H
