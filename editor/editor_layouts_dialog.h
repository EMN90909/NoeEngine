// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef EDITOR_LAYOUTS_DIALOG_H
#define EDITOR_LAYOUTS_DIALOG_H

#include "scene/gui/dialogs.h"

class LineEdit;
class ItemList;

class EditorLayoutsDialog : public ConfirmationDialog {
	GDCLASS(EditorLayoutsDialog, ConfirmationDialog);

	LineEdit *name = nullptr;
	ItemList *layout_names = nullptr;
	VBoxContainer *makevb = nullptr;

	void _line_gui_input(const Ref<InputEvent> &p_event);
	void _update_ok_disable_state();
	void _deselect_layout_names();

protected:
	static void _bind_methods();
	virtual void ok_pressed() override;
	virtual void _post_popup() override;

public:
	EditorLayoutsDialog();

	void set_name_line_enabled(bool p_enabled);
};

#endif // EDITOR_LAYOUTS_DIALOG_H
