// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef EDITOR_DIR_DIALOG_H
#define EDITOR_DIR_DIALOG_H

#include "core/io/dir_access.h"
#include "editor/editor_file_system.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/tree.h"

class EditorDirDialog : public ConfirmationDialog {
	GDCLASS(EditorDirDialog, ConfirmationDialog);

	ConfirmationDialog *makedialog = nullptr;
	LineEdit *makedirname = nullptr;
	AcceptDialog *mkdirerr = nullptr;

	Button *makedir = nullptr;
	HashSet<String> opened_paths;

	Tree *tree = nullptr;
	bool updating = false;

	void _item_collapsed(Object *p_item);
	void _item_activated();
	void _update_dir(TreeItem *p_item, EditorFileSystemDirectory *p_dir, const String &p_select_path = String());

	void _make_dir();
	void _make_dir_confirm();

	void ok_pressed() override;

	bool must_reload = false;

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	void reload(const String &p_path = "");
	EditorDirDialog();
};

#endif // EDITOR_DIR_DIALOG_H
