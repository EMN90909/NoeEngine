// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef EDITOR_SCENE_EXPORTER_GLTF_PLUGIN_H
#define EDITOR_SCENE_EXPORTER_GLTF_PLUGIN_H

#ifdef TOOLS_ENABLED

#include "editor/editor_plugin.h"
#include "editor_scene_importer_gltf.h"

class EditorFileDialog;

class SceneExporterGLTFPlugin : public EditorPlugin {
	GDCLASS(SceneExporterGLTFPlugin, EditorPlugin);

	EditorFileDialog *file_export_lib = nullptr;
	void _gltf2_dialog_action(String p_file);
	void convert_scene_to_gltf2();

public:
	virtual String get_name() const override;
	bool has_main_screen() const override;
	SceneExporterGLTFPlugin();
};

#endif // TOOLS_ENABLED

#endif // EDITOR_SCENE_EXPORTER_GLTF_PLUGIN_H
