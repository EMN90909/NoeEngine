// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef THEME_OWNER_H
#define THEME_OWNER_H

#include "core/object/object.h"
#include "scene/resources/theme.h"

class Control;
class Node;
class Window;

class ThemeOwner : public Object {
	Control *owner_control = nullptr;
	Window *owner_window = nullptr;

	Node *_get_next_owner_node(Node *p_from_node) const;
	Ref<Theme> _get_owner_node_theme(Node *p_owner_node) const;

public:
	// Theme owner node.

	void set_owner_node(Node *p_node);
	Node *get_owner_node() const;
	bool has_owner_node() const;

	// Theme propagation.

	void assign_theme_on_parented(Node *p_for_node);
	void clear_theme_on_unparented(Node *p_for_node);
	void propagate_theme_changed(Node *p_to_node, Node *p_owner_node, bool p_notify, bool p_assign);

	// Theme lookup.

	void get_theme_type_dependencies(const Node *p_for_node, const StringName &p_theme_type, List<StringName> *r_list) const;

	Variant get_theme_item_in_types(Theme::DataType p_data_type, const StringName &p_name, List<StringName> p_theme_types);
	bool has_theme_item_in_types(Theme::DataType p_data_type, const StringName &p_name, List<StringName> p_theme_types);

	float get_theme_default_base_scale();
	Ref<Font> get_theme_default_font();
	int get_theme_default_font_size();

	ThemeOwner() {}
	~ThemeOwner() {}
};

#endif // THEME_OWNER_H
