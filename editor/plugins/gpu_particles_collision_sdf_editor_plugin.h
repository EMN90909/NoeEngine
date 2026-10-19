// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef GPU_PARTICLES_COLLISION_SDF_EDITOR_PLUGIN_H
#define GPU_PARTICLES_COLLISION_SDF_EDITOR_PLUGIN_H

#include "editor/editor_plugin.h"
#include "scene/3d/gpu_particles_collision_3d.h"
#include "scene/resources/material.h"

struct EditorProgress;
class EditorFileDialog;
class HBoxContainer;

class GPUParticlesCollisionSDF3DEditorPlugin : public EditorPlugin {
	GDCLASS(GPUParticlesCollisionSDF3DEditorPlugin, EditorPlugin);

	GPUParticlesCollisionSDF3D *col_sdf = nullptr;

	HBoxContainer *bake_hb = nullptr;
	Button *bake = nullptr;

	EditorFileDialog *probe_file = nullptr;

	static EditorProgress *tmp_progress;
	static void bake_func_begin(int p_steps);
	static void bake_func_step(int p_step, const String &p_description);
	static void bake_func_end();

	void _bake();
	void _sdf_save_path_and_bake(const String &p_path);

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	virtual String get_name() const override { return "GPUParticlesCollisionSDF3D"; }
	bool has_main_screen() const override { return false; }
	virtual void edit(Object *p_object) override;
	virtual bool handles(Object *p_object) const override;
	virtual void make_visible(bool p_visible) override;

	GPUParticlesCollisionSDF3DEditorPlugin();
	~GPUParticlesCollisionSDF3DEditorPlugin();
};

#endif // GPU_PARTICLES_COLLISION_SDF_EDITOR_PLUGIN_H
