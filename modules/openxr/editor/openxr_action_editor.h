// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef OPENXR_ACTION_EDITOR_H
#define OPENXR_ACTION_EDITOR_H

#include "../action_map/openxr_action.h"
#include "editor/editor_undo_redo_manager.h"
#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/line_edit.h"
#include "scene/gui/option_button.h"
#include "scene/gui/text_edit.h"

class OpenXRActionEditor : public HBoxContainer {
	GDCLASS(OpenXRActionEditor, HBoxContainer);

private:
	EditorUndoRedoManager *undo_redo;
	Ref<OpenXRAction> action;

	LineEdit *action_name = nullptr;
	LineEdit *action_localized_name = nullptr;
	OptionButton *action_type_button = nullptr;
	Button *rem_action = nullptr;

	void _theme_changed();
	void _on_action_name_changed(const String p_new_text);
	void _on_action_localized_name_changed(const String p_new_text);
	void _on_item_selected(int p_idx);
	void _on_remove_action();

protected:
	static void _bind_methods();
	void _notification(int p_what);

	// used for undo/redo
	void _do_set_name(const String p_new_text);
	void _do_set_localized_name(const String p_new_text);
	void _do_set_action_type(OpenXRAction::ActionType p_action_type);

public:
	Ref<OpenXRAction> get_action() { return action; };
	OpenXRActionEditor(Ref<OpenXRAction> p_action);
};

#endif // OPENXR_ACTION_EDITOR_H
