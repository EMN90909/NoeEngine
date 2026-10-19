// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef GLTF_LIGHT_H
#define GLTF_LIGHT_H

#include "core/io/resource.h"

class Light3D;

// https://github.com/KhronosGroup/glTF/blob/main/extensions/2.0/Khronos/KHR_lights_punctual

class GLTFLight : public Resource {
	GDCLASS(GLTFLight, Resource)
	friend class GLTFDocument;

protected:
	static void _bind_methods();

private:
	Color color = Color(1.0f, 1.0f, 1.0f);
	float intensity = 1.0f;
	String light_type;
	float range = INFINITY;
	float inner_cone_angle = 0.0f;
	float outer_cone_angle = Math_TAU / 8.0f;

public:
	Color get_color();
	void set_color(Color p_color);

	float get_intensity();
	void set_intensity(float p_intensity);

	String get_light_type();
	void set_light_type(String p_light_type);

	float get_range();
	void set_range(float p_range);

	float get_inner_cone_angle();
	void set_inner_cone_angle(float p_inner_cone_angle);

	float get_outer_cone_angle();
	void set_outer_cone_angle(float p_outer_cone_angle);

	static Ref<GLTFLight> from_node(const Light3D *p_light);
	Light3D *to_node() const;

	static Ref<GLTFLight> from_dictionary(const Dictionary p_dictionary);
	Dictionary to_dictionary() const;
};

#endif // GLTF_LIGHT_H
