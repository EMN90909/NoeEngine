// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef VOXEL_GI_EDITOR_PLUGIN_H
#define VOXEL_GI_EDITOR_PLUGIN_H

#include "editor/editor_plugin.h"
#include "scene/3d/voxel_gi.h"
#include "scene/resources/material.h"

class EditorFileDialog;
struct EditorProgress;
class HBoxContainer;

class VoxelGIEditorPlugin : public EditorPlugin {
	GDCLASS(VoxelGIEditorPlugin, EditorPlugin);

	VoxelGI *voxel_gi = nullptr;

	HBoxContainer *bake_hb = nullptr;
	Button *bake = nullptr;

	EditorFileDialog *probe_file = nullptr;

	static EditorProgress *tmp_progress;
	static void bake_func_begin(int p_steps);
	static void bake_func_step(int p_step, const String &p_description);
	static void bake_func_end();

	void _bake();
	void _voxel_gi_save_path_and_bake(const String &p_path);

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	virtual String get_name() const override { return "VoxelGI"; }
	bool has_main_screen() const override { return false; }
	virtual void edit(Object *p_object) override;
	virtual bool handles(Object *p_object) const override;
	virtual void make_visible(bool p_visible) override;

	VoxelGIEditorPlugin();
	~VoxelGIEditorPlugin();
};

#endif // VOXEL_GI_EDITOR_PLUGIN_H
