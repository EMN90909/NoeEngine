// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "gltf_animation.h"

void GLTFAnimation::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_loop"), &GLTFAnimation::get_loop);
	ClassDB::bind_method(D_METHOD("set_loop", "loop"), &GLTFAnimation::set_loop);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "loop"), "set_loop", "get_loop"); // bool
}

bool GLTFAnimation::get_loop() const {
	return loop;
}

void GLTFAnimation::set_loop(bool p_val) {
	loop = p_val;
}

HashMap<int, GLTFAnimation::Track> &GLTFAnimation::get_tracks() {
	return tracks;
}

GLTFAnimation::GLTFAnimation() {
}
