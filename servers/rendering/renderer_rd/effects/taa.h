// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef TAA_RD_H
#define TAA_RD_H

#include "servers/rendering/renderer_rd/pipeline_cache_rd.h"
#include "servers/rendering/renderer_rd/shaders/effects/taa_resolve.glsl.gen.h"
#include "servers/rendering/renderer_rd/storage_rd/render_scene_buffers_rd.h"
#include "servers/rendering/renderer_scene_render.h"

#include "servers/rendering_server.h"

namespace RendererRD {

class TAA {
public:
	TAA();
	~TAA();

	void msaa_resolve(Ref<RenderSceneBuffersRD> p_render_buffers);
	void process(Ref<RenderSceneBuffersRD> p_render_buffers, RD::DataFormat p_format, float p_z_near, float p_z_far);

private:
	struct TAAResolvePushConstant {
		float resolution_width;
		float resolution_height;
		float disocclusion_threshold;
		float disocclusion_scale;
	};

	TaaResolveShaderRD taa_shader;
	RID shader_version;
	RID pipeline;

	void resolve(RID p_frame, RID p_temp, RID p_depth, RID p_velocity, RID p_prev_velocity, RID p_history, Size2 p_resolution, float p_z_near, float p_z_far);
};

} // namespace RendererRD

#endif // TAA_RD_H
