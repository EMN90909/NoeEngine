// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef PROPERTY_SELECTOR_H
#define PROPERTY_SELECTOR_H

#include "scene/gui/dialogs.h"

class EditorHelpBit;
class LineEdit;
class Tree;

class PropertySelector : public ConfirmationDialog {
	GDCLASS(PropertySelector, ConfirmationDialog);

	LineEdit *search_box = nullptr;
	Tree *search_options = nullptr;

	void _text_changed(const String &p_newtext);
	void _sbox_input(const Ref<InputEvent> &p_ie);
	void _update_search();
	void _confirmed();
	void _item_selected();
	void _hide_requested();

	EditorHelpBit *help_bit = nullptr;

	bool properties = false;
	String selected;
	Variant::Type type;
	String base_type;
	ObjectID script;
	Object *instance = nullptr;
	bool virtuals_only = false;

	Vector<Variant::Type> type_filter;

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	void select_method_from_base_type(const String &p_base, const String &p_current = "", bool p_virtuals_only = false);
	void select_method_from_script(const Ref<Script> &p_script, const String &p_current = "");
	void select_method_from_basic_type(Variant::Type p_type, const String &p_current = "");
	void select_method_from_instance(Object *p_instance, const String &p_current = "");

	void select_property_from_base_type(const String &p_base, const String &p_current = "");
	void select_property_from_script(const Ref<Script> &p_script, const String &p_current = "");
	void select_property_from_basic_type(Variant::Type p_type, const String &p_current = "");
	void select_property_from_instance(Object *p_instance, const String &p_current = "");

	void set_type_filter(const Vector<Variant::Type> &p_type_filter);

	PropertySelector();
};

#endif // PROPERTY_SELECTOR_H
