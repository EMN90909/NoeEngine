// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef PROJECT_SETTINGS_EDITOR_H
#define PROJECT_SETTINGS_EDITOR_H

#include "core/config/project_settings.h"
#include "editor/action_map_editor.h"
#include "editor/editor_autoload_settings.h"
#include "editor/editor_data.h"
#include "editor/editor_plugin_settings.h"
#include "editor/editor_sectioned_inspector.h"
#include "editor/import_defaults_editor.h"
#include "editor/localization_editor.h"
#include "editor/shader_globals_editor.h"
#include "scene/gui/tab_container.h"

class FileSystemDock;

class ProjectSettingsEditor : public AcceptDialog {
	GDCLASS(ProjectSettingsEditor, AcceptDialog);

	static ProjectSettingsEditor *singleton;
	ProjectSettings *ps = nullptr;
	Timer *timer = nullptr;

	TabContainer *tab_container = nullptr;
	VBoxContainer *general_editor = nullptr;
	SectionedInspector *general_settings_inspector = nullptr;
	ActionMapEditor *action_map_editor = nullptr;
	LocalizationEditor *localization_editor = nullptr;
	EditorAutoloadSettings *autoload_settings = nullptr;
	ShaderGlobalsEditor *shaders_global_shader_uniforms_editor = nullptr;
	EditorPluginSettings *plugin_settings = nullptr;

	LineEdit *search_box = nullptr;
	CheckButton *advanced = nullptr;

	HBoxContainer *custom_properties = nullptr;
	LineEdit *property_box = nullptr;
	OptionButton *feature_box = nullptr;
	OptionButton *type_box = nullptr;
	Button *add_button = nullptr;
	Button *del_button = nullptr;

	Label *restart_label = nullptr;
	TextureRect *restart_icon = nullptr;
	PanelContainer *restart_container = nullptr;
	Button *restart_close_button = nullptr;

	ImportDefaultsEditor *import_defaults_editor = nullptr;
	EditorData *data = nullptr;

	void _advanced_toggled(bool p_button_pressed);
	void _update_advanced(bool p_is_advanced);
	void _property_box_changed(const String &p_text);
	void _update_property_box();
	void _feature_selected(int p_index);
	void _select_type(Variant::Type p_type);

	virtual void shortcut_input(const Ref<InputEvent> &p_event) override;

	String _get_setting_name() const;
	void _setting_edited(const String &p_name);
	void _setting_selected(const String &p_path);
	void _add_setting();
	void _delete_setting();

	void _editor_restart_request();
	void _editor_restart();
	void _editor_restart_close();

	void _add_feature_overrides();

	void _action_added(const String &p_name);
	void _action_edited(const String &p_name, const Dictionary &p_action);
	void _action_removed(const String &p_name);
	void _action_renamed(const String &p_old_name, const String &p_new_name);
	void _action_reordered(const String &p_action_name, const String &p_relative_to, bool p_before);
	void _update_action_map_editor();
	void _update_theme();

	void _input_filter_focused();
	void _input_filter_unfocused();

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	static ProjectSettingsEditor *get_singleton() { return singleton; }
	void popup_project_settings(bool p_clear_filter = false);
	void set_plugins_page();
	void set_general_page(const String &p_category);
	void update_plugins();

	EditorAutoloadSettings *get_autoload_settings() { return autoload_settings; }
	TabContainer *get_tabs() { return tab_container; }

	void queue_save();
	void connect_filesystem_dock_signals(FileSystemDock *p_fs_dock);

	ProjectSettingsEditor(EditorData *p_data);
};

#endif // PROJECT_SETTINGS_EDITOR_H
