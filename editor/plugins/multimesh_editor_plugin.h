// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef MULTIMESH_EDITOR_PLUGIN_H
#define MULTIMESH_EDITOR_PLUGIN_H

#include "editor/editor_plugin.h"
#include "scene/3d/multimesh_instance_3d.h"
#include "scene/gui/slider.h"
#include "scene/gui/spin_box.h"

class AcceptDialog;
class ConfirmationDialog;
class MenuButton;
class OptionButton;
class SceneTreeDialog;

class MultiMeshEditor : public Control {
	GDCLASS(MultiMeshEditor, Control);

	friend class MultiMeshEditorPlugin;

	AcceptDialog *err_dialog = nullptr;
	MenuButton *options = nullptr;
	MultiMeshInstance3D *_last_pp_node = nullptr;
	bool browsing_source = false;

	Panel *panel = nullptr;
	MultiMeshInstance3D *node = nullptr;

	LineEdit *surface_source = nullptr;
	LineEdit *mesh_source = nullptr;

	SceneTreeDialog *std = nullptr;

	ConfirmationDialog *populate_dialog = nullptr;
	OptionButton *populate_axis = nullptr;
	HSlider *populate_rotate_random = nullptr;
	HSlider *populate_tilt_random = nullptr;
	SpinBox *populate_scale_random = nullptr;
	SpinBox *populate_scale = nullptr;
	SpinBox *populate_amount = nullptr;

	enum Menu {
		MENU_OPTION_POPULATE
	};

	void _browsed(const NodePath &p_path);
	void _menu_option(int);
	void _populate();
	void _browse(bool p_source);

protected:
	void _node_removed(Node *p_node);
	static void _bind_methods();

public:
	void edit(MultiMeshInstance3D *p_multimesh);
	MultiMeshEditor();
};

class MultiMeshEditorPlugin : public EditorPlugin {
	GDCLASS(MultiMeshEditorPlugin, EditorPlugin);

	MultiMeshEditor *multimesh_editor = nullptr;

public:
	virtual String get_name() const override { return "MultiMesh"; }
	bool has_main_screen() const override { return false; }
	virtual void edit(Object *p_object) override;
	virtual bool handles(Object *p_object) const override;
	virtual void make_visible(bool p_visible) override;

	MultiMeshEditorPlugin();
	~MultiMeshEditorPlugin();
};

#endif // MULTIMESH_EDITOR_PLUGIN_H
