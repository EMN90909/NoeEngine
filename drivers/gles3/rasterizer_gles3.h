// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef RASTERIZER_GLES3_H
#define RASTERIZER_GLES3_H

#ifdef GLES3_ENABLED

#include "effects/copy_effects.h"
#include "environment/fog.h"
#include "environment/gi.h"
#include "rasterizer_canvas_gles3.h"
#include "rasterizer_scene_gles3.h"
#include "servers/rendering/renderer_compositor.h"
#include "storage/config.h"
#include "storage/light_storage.h"
#include "storage/material_storage.h"
#include "storage/mesh_storage.h"
#include "storage/particles_storage.h"
#include "storage/texture_storage.h"
#include "storage/utilities.h"

class RasterizerGLES3 : public RendererCompositor {
private:
	uint64_t frame = 1;
	float delta = 0;

	double time_total = 0.0;

protected:
	GLES3::Config *config = nullptr;
	GLES3::Utilities *utilities = nullptr;
	GLES3::TextureStorage *texture_storage = nullptr;
	GLES3::MaterialStorage *material_storage = nullptr;
	GLES3::MeshStorage *mesh_storage = nullptr;
	GLES3::ParticlesStorage *particles_storage = nullptr;
	GLES3::LightStorage *light_storage = nullptr;
	GLES3::GI *gi = nullptr;
	GLES3::Fog *fog = nullptr;
	GLES3::CopyEffects *copy_effects = nullptr;
	RasterizerCanvasGLES3 *canvas = nullptr;
	RasterizerSceneGLES3 *scene = nullptr;
	static RasterizerGLES3 *singleton;

	void _blit_render_target_to_screen(RID p_render_target, DisplayServer::WindowID p_screen, const Rect2 &p_screen_rect, uint32_t p_layer, bool p_first = true);

public:
	RendererUtilities *get_utilities() { return utilities; }
	RendererLightStorage *get_light_storage() { return light_storage; }
	RendererMaterialStorage *get_material_storage() { return material_storage; }
	RendererMeshStorage *get_mesh_storage() { return mesh_storage; }
	RendererParticlesStorage *get_particles_storage() { return particles_storage; }
	RendererTextureStorage *get_texture_storage() { return texture_storage; }
	RendererGI *get_gi() { return gi; }
	RendererFog *get_fog() { return fog; }
	RendererCanvasRender *get_canvas() { return canvas; }
	RendererSceneRender *get_scene() { return scene; }

	void set_boot_image(const Ref<Image> &p_image, const Color &p_color, bool p_scale, bool p_use_filter = true);

	void initialize();
	void begin_frame(double frame_step);

	void prepare_for_blitting_render_targets();
	void blit_render_targets_to_screen(DisplayServer::WindowID p_screen, const BlitToScreen *p_render_targets, int p_amount);

	void end_frame(bool p_swap_buffers);

	void finalize();

	static RendererCompositor *_create_current() {
		return memnew(RasterizerGLES3);
	}

	static void make_current() {
		_create_func = _create_current;
		low_end = true;
	}

	_ALWAYS_INLINE_ uint64_t get_frame_number() const { return frame; }
	_ALWAYS_INLINE_ double get_frame_delta_time() const { return delta; }
	_ALWAYS_INLINE_ double get_total_time() const { return time_total; }

	static RasterizerGLES3 *get_singleton() { return singleton; }
	RasterizerGLES3();
	~RasterizerGLES3();
};

#endif // GLES3_ENABLED

#endif // RASTERIZER_GLES3_H
