// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "gltf_texture.h"

void GLTFTexture::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_src_image"), &GLTFTexture::get_src_image);
	ClassDB::bind_method(D_METHOD("set_src_image", "src_image"), &GLTFTexture::set_src_image);
	ClassDB::bind_method(D_METHOD("get_sampler"), &GLTFTexture::get_sampler);
	ClassDB::bind_method(D_METHOD("set_sampler", "sampler"), &GLTFTexture::set_sampler);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "src_image"), "set_src_image", "get_src_image"); // int
	ADD_PROPERTY(PropertyInfo(Variant::INT, "sampler"), "set_sampler", "get_sampler"); // int
}

GLTFImageIndex GLTFTexture::get_src_image() const {
	return src_image;
}

void GLTFTexture::set_src_image(GLTFImageIndex val) {
	src_image = val;
}

GLTFTextureSamplerIndex GLTFTexture::get_sampler() const {
	return sampler;
}

void GLTFTexture::set_sampler(GLTFTextureSamplerIndex val) {
	sampler = val;
}
