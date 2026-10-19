// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef OPENXR_ACTION_SET_EDITOR_H
#define OPENXR_ACTION_SET_EDITOR_H

#include "../action_map/openxr_action_map.h"
#include "../action_map/openxr_action_set.h"
#include "openxr_action_editor.h"
#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/line_edit.h"
#include "scene/gui/panel_container.h"
#include "scene/gui/text_edit.h"

class OpenXRActionSetEditor : public HBoxContainer {
	GDCLASS(OpenXRActionSetEditor, HBoxContainer);

private:
	EditorUndoRedoManager *undo_redo;
	Ref<OpenXRActionMap> action_map;
	Ref<OpenXRActionSet> action_set;

	bool is_expanded = true;

	PanelContainer *panel = nullptr;
	Button *fold_btn = nullptr;
	VBoxContainer *main_vb = nullptr;
	HBoxContainer *action_set_hb = nullptr;
	LineEdit *action_set_name = nullptr;
	LineEdit *action_set_localized_name = nullptr;
	TextEdit *action_set_priority = nullptr;
	Button *add_action = nullptr;
	Button *rem_action_set = nullptr;
	VBoxContainer *actions_vb = nullptr;

	void _set_fold_icon();
	void _theme_changed();
	OpenXRActionEditor *_add_action_editor(Ref<OpenXRAction> p_action);

	void _on_toggle_expand();
	void _on_action_set_name_changed(const String p_new_text);
	void _on_action_set_localized_name_changed(const String p_new_text);
	void _on_action_set_priority_changed(const String p_new_text);
	void _on_add_action();
	void _on_remove_action_set();

	void _on_remove_action(Object *p_action_editor);

protected:
	static void _bind_methods();
	void _notification(int p_what);

	// used for undo/redo
	void _do_set_name(const String p_new_text);
	void _do_set_localized_name(const String p_new_text);
	void _do_set_priority(int64_t value);
	void _do_add_action_editor(OpenXRActionEditor *p_action_editor);
	void _do_remove_action_editor(OpenXRActionEditor *p_action_editor);

public:
	Ref<OpenXRActionSet> get_action_set() { return action_set; };
	void set_focus_on_entry();

	void remove_all_actions();

	OpenXRActionSetEditor(Ref<OpenXRActionMap> p_action_map, Ref<OpenXRActionSet> p_action_set);
};

#endif // OPENXR_ACTION_SET_EDITOR_H
