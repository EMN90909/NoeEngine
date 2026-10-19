// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef INPUT_EVENT_CONFIGURATION_DIALOG_H
#define INPUT_EVENT_CONFIGURATION_DIALOG_H

#include "scene/gui/dialogs.h"

class OptionButton;
class Tree;
class EventListenerLineEdit;
class CheckBox;

// Confirmation Dialog used when configuring an input event.
// Separate from ActionMapEditor for code cleanliness and separation of responsibilities.
class InputEventConfigurationDialog : public ConfirmationDialog {
	GDCLASS(InputEventConfigurationDialog, ConfirmationDialog)
private:
	struct IconCache {
		Ref<Texture2D> keyboard;
		Ref<Texture2D> mouse;
		Ref<Texture2D> joypad_button;
		Ref<Texture2D> joypad_axis;
	} icon_cache;

	Ref<InputEvent> event;
	Ref<InputEvent> original_event;

	bool in_tree_update = false;

	// Listening for input
	EventListenerLineEdit *event_listener = nullptr;
	Label *event_as_text = nullptr;

	// List of All Key/Mouse/Joypad input options.
	int allowed_input_types;
	Tree *input_list_tree = nullptr;
	LineEdit *input_list_search = nullptr;

	// Additional Options, shown depending on event selected
	VBoxContainer *additional_options_container = nullptr;

	HBoxContainer *device_container = nullptr;
	OptionButton *device_id_option = nullptr;

	HBoxContainer *mod_container = nullptr; // Contains the subcontainer and the store command checkbox.

	enum ModCheckbox {
		MOD_ALT,
		MOD_SHIFT,
		MOD_CTRL,
		MOD_META,
		MOD_MAX
	};
#if defined(MACOS_ENABLED)
	String mods[MOD_MAX] = { "Option", "Shift", "Ctrl", "Command" };
#elif defined(WINDOWS_ENABLED)
	String mods[MOD_MAX] = { "Alt", "Shift", "Ctrl", "Windows" };
#else
	String mods[MOD_MAX] = { "Alt", "Shift", "Ctrl", "Meta" };
#endif
	String mods_tip[MOD_MAX] = { "Alt or Option key", "Shift key", "Control key", "Meta/Windows or Command key" };

	CheckBox *mod_checkboxes[MOD_MAX];
	CheckBox *autoremap_command_or_control_checkbox = nullptr;

	enum KeyMode {
		KEYMODE_KEYCODE,
		KEYMODE_PHY_KEYCODE,
		KEYMODE_UNICODE,
	};

	OptionButton *key_mode = nullptr;

	void _set_event(const Ref<InputEvent> &p_event, const Ref<InputEvent> &p_original_event, bool p_update_input_list_selection = true);
	void _on_listen_input_changed(const Ref<InputEvent> &p_event);
	void _on_listen_focus_changed();

	void _search_term_updated(const String &p_term);
	void _update_input_list();
	void _input_list_item_selected();

	void _mod_toggled(bool p_checked, int p_index);
	void _autoremap_command_or_control_toggled(bool p_checked);
	void _key_mode_selected(int p_mode);

	void _device_selection_changed(int p_option_button_index);
	void _set_current_device(int p_device);
	int _get_current_device() const;

protected:
	void _notification(int p_what);

public:
	// Pass an existing event to configure it. Alternatively, pass no event to start with a blank configuration.
	void popup_and_configure(const Ref<InputEvent> &p_event = Ref<InputEvent>());
	Ref<InputEvent> get_event() const;

	void set_allowed_input_types(int p_type_masks);

	InputEventConfigurationDialog();
};

#endif // INPUT_EVENT_CONFIGURATION_DIALOG_H
