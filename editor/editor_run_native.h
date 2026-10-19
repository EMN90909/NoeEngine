// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef EDITOR_RUN_NATIVE_H
#define EDITOR_RUN_NATIVE_H

#include "scene/gui/box_container.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/menu_button.h"
#include "scene/gui/rich_text_label.h"

class EditorRunNative : public HBoxContainer {
	GDCLASS(EditorRunNative, HBoxContainer);

	RichTextLabel *result_dialog_log = nullptr;
	AcceptDialog *result_dialog = nullptr;

	MenuButton *remote_debug = nullptr;
	bool first = true;

	int resume_id = -1;

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	Error run_native(int p_id);
	bool is_deploy_debug_remote_enabled() const;

	void resume_run_native();

	EditorRunNative();
};

#endif // EDITOR_RUN_NATIVE_H
