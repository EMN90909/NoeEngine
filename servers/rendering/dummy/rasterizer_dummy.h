// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef RASTERIZER_DUMMY_H
#define RASTERIZER_DUMMY_H

#include "core/templates/rid_owner.h"
#include "core/templates/self_list.h"
#include "scene/resources/mesh.h"
#include "servers/rendering/dummy/environment/fog.h"
#include "servers/rendering/dummy/environment/gi.h"
#include "servers/rendering/dummy/rasterizer_canvas_dummy.h"
#include "servers/rendering/dummy/rasterizer_scene_dummy.h"
#include "servers/rendering/dummy/storage/light_storage.h"
#include "servers/rendering/dummy/storage/material_storage.h"
#include "servers/rendering/dummy/storage/mesh_storage.h"
#include "servers/rendering/dummy/storage/particles_storage.h"
#include "servers/rendering/dummy/storage/texture_storage.h"
#include "servers/rendering/dummy/storage/utilities.h"
#include "servers/rendering/renderer_compositor.h"
#include "servers/rendering_server.h"

class RasterizerDummy : public RendererCompositor {
private:
	uint64_t frame = 1;
	double delta = 0;
	double time = 0.0;

protected:
	RasterizerCanvasDummy canvas;
	RendererDummy::Utilities utilities;
	RendererDummy::LightStorage light_storage;
	RendererDummy::MaterialStorage material_storage;
	RendererDummy::MeshStorage mesh_storage;
	RendererDummy::ParticlesStorage particles_storage;
	RendererDummy::TextureStorage texture_storage;
	RendererDummy::GI gi;
	RendererDummy::Fog fog;
	RasterizerSceneDummy scene;

public:
	RendererUtilities *get_utilities() override { return &utilities; };
	RendererLightStorage *get_light_storage() override { return &light_storage; };
	RendererMaterialStorage *get_material_storage() override { return &material_storage; };
	RendererMeshStorage *get_mesh_storage() override { return &mesh_storage; };
	RendererParticlesStorage *get_particles_storage() override { return &particles_storage; };
	RendererTextureStorage *get_texture_storage() override { return &texture_storage; };
	RendererGI *get_gi() override { return &gi; };
	RendererFog *get_fog() override { return &fog; };
	RendererCanvasRender *get_canvas() override { return &canvas; }
	RendererSceneRender *get_scene() override { return &scene; }

	void set_boot_image(const Ref<Image> &p_image, const Color &p_color, bool p_scale, bool p_use_filter = true) override {}

	void initialize() override {}
	void begin_frame(double frame_step) override {
		frame++;
		delta = frame_step;
		time += frame_step;
	}

	void prepare_for_blitting_render_targets() override {}
	void blit_render_targets_to_screen(int p_screen, const BlitToScreen *p_render_targets, int p_amount) override {}

	void end_frame(bool p_swap_buffers) override {
		if (p_swap_buffers) {
			DisplayServer::get_singleton()->swap_buffers();
		}
	}

	void finalize() override {}

	static RendererCompositor *_create_current() {
		return memnew(RasterizerDummy);
	}

	static void make_current() {
		_create_func = _create_current;
		low_end = true;
	}

	uint64_t get_frame_number() const override { return frame; }
	double get_frame_delta_time() const override { return delta; }
	double get_total_time() const override { return time; }

	RasterizerDummy() {}
	~RasterizerDummy() {}
};

#endif // RASTERIZER_DUMMY_H
