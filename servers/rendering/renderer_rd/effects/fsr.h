// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef FSR_RD_H
#define FSR_RD_H

#include "../pipeline_cache_rd.h"
#include "../shaders/effects/fsr_upscale.glsl.gen.h"
#include "../storage_rd/render_scene_buffers_rd.h"
#include "servers/rendering/renderer_scene_render.h"
#include "servers/rendering_server.h"

namespace RendererRD {

class FSR {
public:
	FSR();
	~FSR();

	void fsr_upscale(Ref<RenderSceneBuffersRD> p_render_buffers, RID p_source_rd_texture, RID p_destination_texture);

private:
	enum FSRUpscalePass {
		FSR_UPSCALE_PASS_EASU = 0,
		FSR_UPSCALE_PASS_RCAS = 1
	};

	struct FSRUpscalePushConstant {
		float resolution_width;
		float resolution_height;
		float upscaled_width;
		float upscaled_height;
		float sharpness;
		int pass;
		int _unused0, _unused1;
	};

	FsrUpscaleShaderRD fsr_shader;
	RID shader_version;
	RID pipeline;
};

} // namespace RendererRD

#endif // FSR_RD_H
