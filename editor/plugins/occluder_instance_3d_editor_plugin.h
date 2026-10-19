// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef OCCLUDER_INSTANCE_3D_EDITOR_PLUGIN_H
#define OCCLUDER_INSTANCE_3D_EDITOR_PLUGIN_H

#include "editor/editor_plugin.h"
#include "scene/3d/occluder_instance_3d.h"
#include "scene/resources/material.h"

class EditorFileDialog;

class OccluderInstance3DEditorPlugin : public EditorPlugin {
	GDCLASS(OccluderInstance3DEditorPlugin, EditorPlugin);

	OccluderInstance3D *occluder_instance = nullptr;

	Button *bake = nullptr;

	EditorFileDialog *file_dialog = nullptr;

	void _bake_select_file(const String &p_file);
	void _bake();

protected:
	static void _bind_methods();

public:
	virtual String get_name() const override { return "OccluderInstance3D"; }
	bool has_main_screen() const override { return false; }
	virtual void edit(Object *p_object) override;
	virtual bool handles(Object *p_object) const override;
	virtual void make_visible(bool p_visible) override;

	OccluderInstance3DEditorPlugin();
	~OccluderInstance3DEditorPlugin();
};

#endif // OCCLUDER_INSTANCE_3D_EDITOR_PLUGIN_H
