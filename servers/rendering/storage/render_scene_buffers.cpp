// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "render_scene_buffers.h"

void RenderSceneBuffers::_bind_methods() {
	ClassDB::bind_method(D_METHOD("configure", "render_target", "internal_size", "target_size", "fsr_sharpness", "texture_mipmap_bias", "msaa", "screen_space_aa", "use_taa", "use_debanding", "view_count"), &RenderSceneBuffers::configure);
}

void RenderSceneBuffers::configure(RID p_render_target, const Size2i p_internal_size, const Size2i p_target_size, RS::ViewportScaling3DMode p_scaling_3d_mode, float p_fsr_sharpness, float p_texture_mipmap_bias, RS::ViewportMSAA p_msaa, RenderingServer::ViewportScreenSpaceAA p_screen_space_aa, bool p_use_taa, bool p_use_debanding, uint32_t p_view_count) {
	GDVIRTUAL_CALL(_configure, p_render_target, p_internal_size, p_target_size, p_scaling_3d_mode, p_fsr_sharpness, p_texture_mipmap_bias, p_msaa, p_screen_space_aa, p_use_taa, p_use_debanding, p_view_count);
};

void RenderSceneBuffers::set_fsr_sharpness(float p_fsr_sharpness) {
	GDVIRTUAL_CALL(_set_fsr_sharpness, p_fsr_sharpness);
}

void RenderSceneBuffers::set_texture_mipmap_bias(float p_texture_mipmap_bias) {
	GDVIRTUAL_CALL(_set_texture_mipmap_bias, p_texture_mipmap_bias);
}

void RenderSceneBuffers::set_use_debanding(bool p_use_debanding) {
	GDVIRTUAL_CALL(_set_use_debanding, p_use_debanding);
}
