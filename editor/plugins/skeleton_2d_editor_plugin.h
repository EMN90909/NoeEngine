// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef SKELETON_2D_EDITOR_PLUGIN_H
#define SKELETON_2D_EDITOR_PLUGIN_H

#include "editor/editor_plugin.h"
#include "scene/2d/skeleton_2d.h"
#include "scene/gui/spin_box.h"

class AcceptDialog;
class MenuButton;

class Skeleton2DEditor : public Control {
	GDCLASS(Skeleton2DEditor, Control);

	enum Menu {
		MENU_OPTION_SET_REST,
		MENU_OPTION_MAKE_REST,
	};

	Skeleton2D *node = nullptr;

	MenuButton *options = nullptr;
	AcceptDialog *err_dialog = nullptr;

	void _menu_option(int p_option);

	//void _create_uv_lines();
	friend class Skeleton2DEditorPlugin;

protected:
	void _node_removed(Node *p_node);
	static void _bind_methods();

public:
	void edit(Skeleton2D *p_sprite);
	Skeleton2DEditor();
};

class Skeleton2DEditorPlugin : public EditorPlugin {
	GDCLASS(Skeleton2DEditorPlugin, EditorPlugin);

	Skeleton2DEditor *sprite_editor = nullptr;

public:
	virtual String get_name() const override { return "Skeleton2D"; }
	bool has_main_screen() const override { return false; }
	virtual void edit(Object *p_object) override;
	virtual bool handles(Object *p_object) const override;
	virtual void make_visible(bool p_visible) override;

	Skeleton2DEditorPlugin();
	~Skeleton2DEditorPlugin();
};

#endif // SKELETON_2D_EDITOR_PLUGIN_H
