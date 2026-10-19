// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef SHADER_CREATE_DIALOG_H
#define SHADER_CREATE_DIALOG_H

#include "editor/editor_settings.h"
#include "scene/gui/check_box.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/grid_container.h"
#include "scene/gui/line_edit.h"
#include "scene/gui/option_button.h"
#include "scene/gui/panel_container.h"

class EditorFileDialog;

class ShaderCreateDialog : public ConfirmationDialog {
	GDCLASS(ShaderCreateDialog, ConfirmationDialog);

	struct ShaderTypeData {
		List<String> extensions;
		String default_extension;
		bool use_templates = false;
	};

	List<ShaderTypeData> type_data;

	GridContainer *gc = nullptr;
	Label *error_label = nullptr;
	Label *path_error_label = nullptr;
	Label *builtin_warning_label = nullptr;
	PanelContainer *status_panel = nullptr;
	OptionButton *type_menu = nullptr;
	OptionButton *mode_menu = nullptr;
	OptionButton *template_menu = nullptr;
	CheckBox *internal = nullptr;
	LineEdit *file_path = nullptr;
	Button *path_button = nullptr;
	EditorFileDialog *file_browse = nullptr;
	AcceptDialog *alert = nullptr;

	String initial_base_path;
	bool is_new_shader_created = true;
	bool is_path_valid = false;
	bool is_built_in = false;
	bool built_in_enabled = true;
	bool load_enabled = false;
	bool re_check_path = false;
	int current_type = -1;
	int default_type = -1;
	int current_mode = 0;
	int current_template = 0;

	virtual void _update_language_info();

	void _path_hbox_sorted();
	void _path_changed(const String &p_path = String());
	void _path_submitted(const String &p_path = String());
	void _type_changed(int p_type = 0);
	void _built_in_toggled(bool p_enabled);
	void _template_changed(int p_template = 0);
	void _mode_changed(int p_mode = 0);
	void _browse_path();
	void _file_selected(const String &p_file);
	String _validate_path(const String &p_path);
	virtual void ok_pressed() override;
	void _create_new();
	void _load_exist();
	void _msg_script_valid(bool valid, const String &p_msg = String());
	void _msg_path_valid(bool valid, const String &p_msg = String());
	void _update_dialog();

protected:
	void _update_theme();
	void _notification(int p_what);
	static void _bind_methods();

public:
	void config(const String &p_base_path, bool p_built_in_enabled = true, bool p_load_enabled = true, int p_preferred_type = -1, int p_preferred_mode = -1);
	ShaderCreateDialog();
};

#endif // SHADER_CREATE_DIALOG_H
