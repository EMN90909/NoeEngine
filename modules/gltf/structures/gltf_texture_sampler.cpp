// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "gltf_texture_sampler.h"

void GLTFTextureSampler::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_mag_filter"), &GLTFTextureSampler::get_mag_filter);
	ClassDB::bind_method(D_METHOD("set_mag_filter", "filter_mode"), &GLTFTextureSampler::set_mag_filter);
	ClassDB::bind_method(D_METHOD("get_min_filter"), &GLTFTextureSampler::get_min_filter);
	ClassDB::bind_method(D_METHOD("set_min_filter", "filter_mode"), &GLTFTextureSampler::set_min_filter);
	ClassDB::bind_method(D_METHOD("get_wrap_s"), &GLTFTextureSampler::get_wrap_s);
	ClassDB::bind_method(D_METHOD("set_wrap_s", "wrap_mode"), &GLTFTextureSampler::set_wrap_s);
	ClassDB::bind_method(D_METHOD("get_wrap_t"), &GLTFTextureSampler::get_wrap_t);
	ClassDB::bind_method(D_METHOD("set_wrap_t", "wrap_mode"), &GLTFTextureSampler::set_wrap_t);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "mag_filter"), "set_mag_filter", "get_mag_filter");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "min_filter"), "set_min_filter", "get_min_filter");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "wrap_s"), "set_wrap_s", "get_wrap_s");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "wrap_t"), "set_wrap_t", "get_wrap_t");
}
