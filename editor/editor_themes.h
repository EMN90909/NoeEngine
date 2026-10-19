// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef EDITOR_THEMES_H
#define EDITOR_THEMES_H

#include "scene/resources/texture.h"
#include "scene/resources/theme.h"

// The default icon theme is designed to be used for a dark theme. This map stores
// Color values to convert to other colors for better readability on a light theme.
class EditorColorMap {
	// Godot Color values are used to avoid the ambiguity of strings
	// (where "#ffffff", "fff", and "white" are all equivalent).
	static HashMap<Color, Color> color_conversion_map;
	// The names of the icons to never convert, even if one of their colors
	// are contained in the color map from above.
	static HashSet<StringName> color_conversion_exceptions;

public:
	static void create();
	static void add_conversion_color_pair(const String p_from_color, const String p_to_color);
	static void add_conversion_exception(const StringName p_icon_name);

	static HashMap<Color, Color> &get_color_conversion_map() { return color_conversion_map; };
	static HashSet<StringName> &get_color_conversion_exceptions() { return color_conversion_exceptions; };
};

Ref<Theme> create_editor_theme(Ref<Theme> p_theme = nullptr);

Ref<Theme> create_custom_theme(Ref<Theme> p_theme = nullptr);

String get_default_project_icon();

#endif // EDITOR_THEMES_H
