// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef NAVIGATION_MESH_EDITOR_PLUGIN_H
#define NAVIGATION_MESH_EDITOR_PLUGIN_H

#ifdef TOOLS_ENABLED

#include "editor/editor_plugin.h"

class AcceptDialog;
class HBoxContainer;
class NavigationRegion3D;

class NavigationMeshEditor : public Control {
	friend class NavigationMeshEditorPlugin;

	GDCLASS(NavigationMeshEditor, Control);

	AcceptDialog *err_dialog = nullptr;

	HBoxContainer *bake_hbox = nullptr;
	Button *button_bake = nullptr;
	Button *button_reset = nullptr;
	Label *bake_info = nullptr;

	NavigationRegion3D *node = nullptr;

	void _bake_pressed();
	void _clear_pressed();

protected:
	void _node_removed(Node *p_node);
	static void _bind_methods();
	void _notification(int p_what);

public:
	void edit(NavigationRegion3D *p_nav_region);
	NavigationMeshEditor();
	~NavigationMeshEditor();
};

class NavigationMeshEditorPlugin : public EditorPlugin {
	GDCLASS(NavigationMeshEditorPlugin, EditorPlugin);

	NavigationMeshEditor *navigation_mesh_editor = nullptr;

public:
	virtual String get_name() const override { return "NavigationMesh"; }
	bool has_main_screen() const override { return false; }
	virtual void edit(Object *p_object) override;
	virtual bool handles(Object *p_object) const override;
	virtual void make_visible(bool p_visible) override;

	NavigationMeshEditorPlugin();
	~NavigationMeshEditorPlugin();
};

#endif // TOOLS_ENABLED

#endif // NAVIGATION_MESH_EDITOR_PLUGIN_H
