// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef OPENXR_SELECT_ACTION_DIALOG_H
#define OPENXR_SELECT_ACTION_DIALOG_H

#include "../action_map/openxr_action_map.h"
#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/label.h"
#include "scene/gui/line_edit.h"
#include "scene/gui/scroll_container.h"
#include "scene/gui/separator.h"
#include "scene/gui/text_edit.h"

class OpenXRSelectActionDialog : public ConfirmationDialog {
	GDCLASS(OpenXRSelectActionDialog, ConfirmationDialog);

private:
	Ref<OpenXRActionMap> action_map;
	String selected_action;
	Dictionary action_buttons;

	VBoxContainer *main_vb = nullptr;
	ScrollContainer *scroll = nullptr;

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	void _on_select_action(const String p_action);
	void open();
	virtual void ok_pressed() override;

	OpenXRSelectActionDialog(Ref<OpenXRActionMap> p_action_map);
};

#endif // OPENXR_SELECT_ACTION_DIALOG_H
