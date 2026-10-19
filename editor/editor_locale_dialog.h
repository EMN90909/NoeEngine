// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef EDITOR_LOCALE_DIALOG_H
#define EDITOR_LOCALE_DIALOG_H

#include "core/string/translation.h"
#include "scene/gui/dialogs.h"

class Button;
class HBoxContainer;
class VBoxContainer;
class LineEdit;
class Tree;
class OptionButton;

class EditorLocaleDialog : public ConfirmationDialog {
	GDCLASS(EditorLocaleDialog, ConfirmationDialog);

	enum LocaleFilter {
		SHOW_ALL_LOCALES,
		SHOW_ONLY_SELECTED_LOCALES,
	};

	HBoxContainer *hb_locale = nullptr;
	VBoxContainer *vb_script_list = nullptr;
	OptionButton *filter_mode = nullptr;
	Button *edit_filters = nullptr;
	Button *advanced = nullptr;
	LineEdit *lang_code = nullptr;
	LineEdit *script_code = nullptr;
	LineEdit *country_code = nullptr;
	LineEdit *variant_code = nullptr;
	Tree *lang_list = nullptr;
	Tree *script_list = nullptr;
	Tree *cnt_list = nullptr;

	bool locale_set = false;
	bool updating_lists = false;

protected:
	static void _bind_methods();
	virtual void _post_popup() override;
	virtual void ok_pressed() override;

	void _item_selected();
	void _filter_lang_option_changed();
	void _filter_script_option_changed();
	void _filter_cnt_option_changed();
	void _filter_mode_changed(int p_mode);
	void _edit_filters(bool p_checked);
	void _toggle_advanced(bool p_checked);

	void _update_tree();

public:
	EditorLocaleDialog();

	void set_locale(const String &p_locale);
	void popup_locale_dialog();
};

#endif // EDITOR_LOCALE_DIALOG_H
