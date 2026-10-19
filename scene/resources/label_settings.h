// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef LABEL_SETTINGS_H
#define LABEL_SETTINGS_H

#include "core/io/resource.h"
#include "font.h"

/*************************************************************************/

class LabelSettings : public Resource {
	GDCLASS(LabelSettings, Resource);

	real_t line_spacing = 3;

	Ref<Font> font;
	int font_size = Font::DEFAULT_FONT_SIZE;
	Color font_color = Color(1, 1, 1);

	int outline_size = 0;
	Color outline_color = Color(1, 1, 1);

	int shadow_size = 1;
	Color shadow_color = Color(0, 0, 0, 0);
	Vector2 shadow_offset = Vector2(1, 1);

	void _font_changed();

protected:
	static void _bind_methods();

public:
	void set_line_spacing(real_t p_spacing);
	real_t get_line_spacing() const;

	void set_font(const Ref<Font> &p_font);
	Ref<Font> get_font() const;

	void set_font_size(int p_size);
	int get_font_size() const;

	void set_font_color(const Color &p_color);
	Color get_font_color() const;

	void set_outline_size(int p_size);
	int get_outline_size() const;

	void set_outline_color(const Color &p_color);
	Color get_outline_color() const;

	void set_shadow_size(int p_size);
	int get_shadow_size() const;

	void set_shadow_color(const Color &p_color);
	Color get_shadow_color() const;

	void set_shadow_offset(const Vector2 &p_offset);
	Vector2 get_shadow_offset() const;
};

#endif // LABEL_SETTINGS_H
