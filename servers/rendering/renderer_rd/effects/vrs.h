// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef VRS_RD_H
#define VRS_RD_H

#include "servers/rendering/renderer_rd/pipeline_cache_rd.h"
#include "servers/rendering/renderer_rd/shaders/effects/vrs.glsl.gen.h"
#include "servers/rendering/renderer_scene_render.h"

#include "servers/rendering_server.h"

namespace RendererRD {

class VRS {
private:
	enum VRSMode {
		VRS_DEFAULT,
		VRS_MULTIVIEW,
		VRS_MAX,
	};

	/* we have no push constant here (yet)
	struct VRSPushConstant {

	};
	*/

	struct VRSShader {
		// VRSPushConstant push_constant;
		VrsShaderRD shader;
		RID shader_version;
		PipelineCacheRD pipelines[VRS_MAX];
	} vrs_shader;

public:
	VRS();
	~VRS();

	void copy_vrs(RID p_source_rd_texture, RID p_dest_framebuffer, bool p_multiview = false);

	Size2i get_vrs_texture_size(const Size2i p_base_size) const;
	void update_vrs_texture(RID p_vrs_fb, RID p_render_target);
};

} // namespace RendererRD

#endif // VRS_RD_H
