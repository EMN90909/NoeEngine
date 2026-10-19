// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef MENU_BUTTON_H
#define MENU_BUTTON_H

#include "scene/gui/button.h"
#include "scene/gui/popup_menu.h"

class MenuButton : public Button {
	GDCLASS(MenuButton, Button);

	bool clicked = false;
	bool switch_on_hover = false;
	bool disable_shortcuts = false;
	PopupMenu *popup = nullptr;

	Vector2i mouse_pos_adjusted;

	void _popup_visibility_changed(bool p_visible);

protected:
	void _notification(int p_what);
	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;
	static void _bind_methods();
	virtual void shortcut_input(const Ref<InputEvent> &p_event) override;

public:
	virtual void pressed() override;

	PopupMenu *get_popup() const;
	void show_popup();

	void set_switch_on_hover(bool p_enabled);
	bool is_switch_on_hover();
	void set_disable_shortcuts(bool p_disabled);

	void set_item_count(int p_count);
	int get_item_count() const;

	MenuButton(const String &p_text = String());
	~MenuButton();
};

#endif // MENU_BUTTON_H
