// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef EDITOR_DEBUGGER_TREE_H
#define EDITOR_DEBUGGER_TREE_H

#include "scene/gui/tree.h"

class SceneDebuggerTree;
class EditorFileDialog;

class EditorDebuggerTree : public Tree {
	GDCLASS(EditorDebuggerTree, Tree);

private:
	enum ItemMenu {
		ITEM_MENU_SAVE_REMOTE_NODE,
		ITEM_MENU_COPY_NODE_PATH,
	};

	ObjectID inspected_object_id;
	int debugger_id = 0;
	bool updating_scene_tree = false;
	HashSet<ObjectID> unfold_cache;
	PopupMenu *item_menu = nullptr;
	EditorFileDialog *file_dialog = nullptr;
	String last_filter;

	String _get_path(TreeItem *p_item);
	void _scene_tree_folded(Object *p_obj);
	void _scene_tree_selected();
	void _scene_tree_rmb_selected(const Vector2 &p_position, MouseButton p_button);
	void _item_menu_id_pressed(int p_option);
	void _file_selected(const String &p_file);

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	enum Button {
		BUTTON_SUBSCENE = 0,
		BUTTON_VISIBILITY = 1,
	};

	virtual Variant get_drag_data(const Point2 &p_point) override;

	String get_selected_path();
	ObjectID get_selected_object();
	int get_current_debugger(); // Would love to have one tree for every debugger.
	void update_scene_tree(const SceneDebuggerTree *p_tree, int p_debugger);
	EditorDebuggerTree();
};

#endif // EDITOR_DEBUGGER_TREE_H
