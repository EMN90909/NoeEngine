// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef RENDER_SCENE_BUFFERS_H
#define RENDER_SCENE_BUFFERS_H

#include "core/object/ref_counted.h"
#include "servers/rendering_server.h"

class RenderSceneBuffers : public RefCounted {
	GDCLASS(RenderSceneBuffers, RefCounted);

protected:
	static void _bind_methods();

	GDVIRTUAL11(_configure, RID, Size2i, Size2i, RS::ViewportScaling3DMode, float, float, RS::ViewportMSAA, RenderingServer::ViewportScreenSpaceAA, bool, bool, uint32_t)
	GDVIRTUAL1(_set_fsr_sharpness, float)
	GDVIRTUAL1(_set_texture_mipmap_bias, float)
	GDVIRTUAL1(_set_use_debanding, bool)

public:
	RenderSceneBuffers(){};
	virtual ~RenderSceneBuffers(){};

	virtual void configure(RID p_render_target, const Size2i p_internal_size, const Size2i p_target_size, RS::ViewportScaling3DMode p_scaling_3d_mode, float p_fsr_sharpness, float p_texture_mipmap_bias, RS::ViewportMSAA p_msaa_3d, RenderingServer::ViewportScreenSpaceAA p_screen_space_aa, bool p_use_taa, bool p_use_debanding, uint32_t p_view_count);

	// for those settings that are unlikely to require buffers to be recreated, we'll add setters
	virtual void set_fsr_sharpness(float p_fsr_sharpness);
	virtual void set_texture_mipmap_bias(float p_texture_mipmap_bias);
	virtual void set_use_debanding(bool p_use_debanding);
};

#endif // RENDER_SCENE_BUFFERS_H
