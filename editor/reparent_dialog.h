// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef REPARENT_DIALOG_H
#define REPARENT_DIALOG_H

#include "editor/scene_tree_editor.h"
#include "scene/gui/button.h"
#include "scene/gui/check_box.h"
#include "scene/gui/check_button.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/line_edit.h"

class ReparentDialog : public ConfirmationDialog {
	GDCLASS(ReparentDialog, ConfirmationDialog);

	SceneTreeEditor *tree = nullptr;
	CheckBox *keep_transform = nullptr;

	void _reparent();
	void _cancel();

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	void set_current(const HashSet<Node *> &p_selection);

	ReparentDialog();
	~ReparentDialog();
};

#endif // REPARENT_DIALOG_H
