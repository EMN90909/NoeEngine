// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef LIGHTMAP_GI_EDITOR_PLUGIN_H
#define LIGHTMAP_GI_EDITOR_PLUGIN_H

#include "editor/editor_plugin.h"
#include "scene/3d/lightmap_gi.h"
#include "scene/resources/material.h"

struct EditorProgress;
class EditorFileDialog;

class LightmapGIEditorPlugin : public EditorPlugin {
	GDCLASS(LightmapGIEditorPlugin, EditorPlugin);

	LightmapGI *lightmap = nullptr;

	Button *bake = nullptr;

	EditorFileDialog *file_dialog = nullptr;
	static EditorProgress *tmp_progress;
	static bool bake_func_step(float p_progress, const String &p_description, void *, bool p_refresh);
	static void bake_func_end(uint64_t p_time_started);

	void _bake_select_file(const String &p_file);
	void _bake();

protected:
	static void _bind_methods();

public:
	virtual String get_name() const override { return "LightmapGI"; }
	bool has_main_screen() const override { return false; }
	virtual void edit(Object *p_object) override;
	virtual bool handles(Object *p_object) const override;
	virtual void make_visible(bool p_visible) override;

	LightmapGIEditorPlugin();
	~LightmapGIEditorPlugin();
};

#endif // LIGHTMAP_GI_EDITOR_PLUGIN_H
