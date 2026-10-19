// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef REPLICATION_EDITOR_H
#define REPLICATION_EDITOR_H

#include "editor/editor_plugin.h"
#include "modules/multiplayer/scene_replication_config.h"
#include "scene/gui/box_container.h"

class ConfirmationDialog;
class MultiplayerSynchronizer;
class AcceptDialog;
class LineEdit;
class Tree;
class TreeItem;
class PropertySelector;
class SceneTreeDialog;

class ReplicationEditor : public VBoxContainer {
	GDCLASS(ReplicationEditor, VBoxContainer);

private:
	MultiplayerSynchronizer *current = nullptr;

	AcceptDialog *error_dialog = nullptr;
	ConfirmationDialog *delete_dialog = nullptr;
	Button *add_pick_button = nullptr;
	Button *add_from_path_button = nullptr;
	LineEdit *np_line_edit = nullptr;

	Label *drop_label = nullptr;

	Ref<SceneReplicationConfig> config;
	NodePath deleting;
	Tree *tree = nullptr;

	PropertySelector *prop_selector = nullptr;
	SceneTreeDialog *pick_node = nullptr;
	NodePath adding_node_path;

	Button *pin = nullptr;

	Ref<Texture2D> _get_class_icon(const Node *p_node);

	void _add_pressed();
	void _tree_item_edited();
	void _tree_button_pressed(Object *p_item, int p_column, int p_id, MouseButton p_button);
	void _update_checked(const NodePath &p_prop, int p_column, bool p_checked);
	void _update_config();
	void _dialog_closed(bool p_confirmed);
	void _add_property(const NodePath &p_property, bool p_spawn = true, bool p_sync = true);

	void _pick_node_filter_text_changed(const String &p_newtext);
	void _pick_node_select_recursive(TreeItem *p_item, const String &p_filter, Vector<Node *> &p_select_candidates);
	void _pick_node_filter_input(const Ref<InputEvent> &p_ie);
	void _pick_node_selected(NodePath p_path);

	void _pick_new_property();
	void _pick_node_property_selected(String p_name);

	bool _can_drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from) const;
	void _drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from);

	void _add_sync_property(String p_path);

protected:
	static void _bind_methods();

	void _notification(int p_what);

public:
	void edit(MultiplayerSynchronizer *p_object);
	MultiplayerSynchronizer *get_current() const { return current; }

	Button *get_pin() { return pin; }
	ReplicationEditor();
	~ReplicationEditor() {}
};

#endif // REPLICATION_EDITOR_H
