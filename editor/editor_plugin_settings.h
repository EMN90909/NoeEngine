// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef EDITOR_PLUGIN_SETTINGS_H
#define EDITOR_PLUGIN_SETTINGS_H

#include "editor/editor_data.h"
#include "editor/plugin_config_dialog.h"

class Tree;

class EditorPluginSettings : public VBoxContainer {
	GDCLASS(EditorPluginSettings, VBoxContainer);

	enum {
		BUTTON_PLUGIN_EDIT
	};

	PluginConfigDialog *plugin_config_dialog = nullptr;
	Tree *plugin_list = nullptr;
	bool updating = false;

	void _plugin_activity_changed();
	void _create_clicked();
	void _cell_button_pressed(Object *p_item, int p_column, int p_id, MouseButton p_button);

	static Vector<String> _get_plugins(const String &p_dir);

protected:
	void _notification(int p_what);

	static void _bind_methods();

public:
	void update_plugins();

	EditorPluginSettings();
};

#endif // EDITOR_PLUGIN_SETTINGS_H
