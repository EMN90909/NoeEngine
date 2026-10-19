// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef CAMERA_3D_EDITOR_PLUGIN_H
#define CAMERA_3D_EDITOR_PLUGIN_H

#include "editor/editor_plugin.h"
#include "scene/3d/camera_3d.h"

class Camera3DEditor : public Control {
	GDCLASS(Camera3DEditor, Control);

	Panel *panel = nullptr;
	Button *preview = nullptr;
	Node *node = nullptr;

	void _pressed();

protected:
	void _node_removed(Node *p_node);
	static void _bind_methods();

public:
	void edit(Node *p_camera);
	Camera3DEditor();
};

class Camera3DEditorPlugin : public EditorPlugin {
	GDCLASS(Camera3DEditorPlugin, EditorPlugin);

	//CameraEditor *camera_editor;

public:
	virtual String get_name() const override { return "Camera3D"; }
	bool has_main_screen() const override { return false; }
	virtual void edit(Object *p_object) override;
	virtual bool handles(Object *p_object) const override;
	virtual void make_visible(bool p_visible) override;

	Camera3DEditorPlugin();
	~Camera3DEditorPlugin();
};

#endif // CAMERA_3D_EDITOR_PLUGIN_H
