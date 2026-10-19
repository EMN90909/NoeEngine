// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef MESH_EDITOR_PLUGIN_H
#define MESH_EDITOR_PLUGIN_H

#include "editor/editor_inspector.h"
#include "editor/editor_plugin.h"
#include "scene/3d/camera_3d.h"
#include "scene/3d/light_3d.h"
#include "scene/3d/mesh_instance_3d.h"
#include "scene/gui/subviewport_container.h"
#include "scene/resources/camera_attributes.h"
#include "scene/resources/material.h"

class SubViewport;
class TextureButton;

class MeshEditor : public SubViewportContainer {
	GDCLASS(MeshEditor, SubViewportContainer);

	float rot_x;
	float rot_y;

	SubViewport *viewport = nullptr;
	MeshInstance3D *mesh_instance = nullptr;
	Node3D *rotation = nullptr;
	DirectionalLight3D *light1 = nullptr;
	DirectionalLight3D *light2 = nullptr;
	Camera3D *camera = nullptr;
	Ref<CameraAttributesPractical> camera_attributes;

	Ref<Mesh> mesh;

	TextureButton *light_1_switch = nullptr;
	TextureButton *light_2_switch = nullptr;

	struct ThemeCache {
		Ref<Texture2D> light_1_on;
		Ref<Texture2D> light_1_off;
		Ref<Texture2D> light_2_on;
		Ref<Texture2D> light_2_off;
	} theme_cache;

	void _button_pressed(Node *p_button);
	void _update_rotation();

protected:
	virtual void _update_theme_item_cache() override;
	void _notification(int p_what);
	void gui_input(const Ref<InputEvent> &p_event) override;

public:
	void edit(Ref<Mesh> p_mesh);
	MeshEditor();
};

class EditorInspectorPluginMesh : public EditorInspectorPlugin {
	GDCLASS(EditorInspectorPluginMesh, EditorInspectorPlugin);

public:
	virtual bool can_handle(Object *p_object) override;
	virtual void parse_begin(Object *p_object) override;
};

class MeshEditorPlugin : public EditorPlugin {
	GDCLASS(MeshEditorPlugin, EditorPlugin);

public:
	virtual String get_name() const override { return "Mesh"; }

	MeshEditorPlugin();
};

#endif // MESH_EDITOR_PLUGIN_H
