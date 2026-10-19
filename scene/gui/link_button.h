// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef LINK_BUTTON_H
#define LINK_BUTTON_H

#include "scene/gui/base_button.h"
#include "scene/resources/text_line.h"

class LinkButton : public BaseButton {
	GDCLASS(LinkButton, BaseButton);

public:
	enum UnderlineMode {
		UNDERLINE_MODE_ALWAYS,
		UNDERLINE_MODE_ON_HOVER,
		UNDERLINE_MODE_NEVER
	};

private:
	String text;
	String xl_text;
	Ref<TextLine> text_buf;
	UnderlineMode underline_mode = UNDERLINE_MODE_ALWAYS;
	String uri;

	String language;
	TextDirection text_direction = TEXT_DIRECTION_AUTO;
	TextServer::StructuredTextParser st_parser = TextServer::STRUCTURED_TEXT_DEFAULT;
	Array st_args;

	struct ThemeCache {
		Ref<StyleBox> focus;

		Color font_color;
		Color font_focus_color;
		Color font_pressed_color;
		Color font_hover_color;
		Color font_hover_pressed_color;
		Color font_disabled_color;

		Ref<Font> font;
		int font_size = 0;
		int outline_size = 0;
		Color font_outline_color;

		int underline_spacing = 0;
	} theme_cache;

	void _shape();

protected:
	virtual void pressed() override;
	virtual Size2 get_minimum_size() const override;
	virtual void _update_theme_item_cache() override;
	void _notification(int p_what);
	static void _bind_methods();

public:
	void set_text(const String &p_text);
	String get_text() const;
	void set_uri(const String &p_uri);
	String get_uri() const;

	void set_structured_text_bidi_override(TextServer::StructuredTextParser p_parser);
	TextServer::StructuredTextParser get_structured_text_bidi_override() const;

	void set_structured_text_bidi_override_options(Array p_args);
	Array get_structured_text_bidi_override_options() const;

	void set_text_direction(TextDirection p_text_direction);
	TextDirection get_text_direction() const;

	void set_language(const String &p_language);
	String get_language() const;

	void set_underline_mode(UnderlineMode p_underline_mode);
	UnderlineMode get_underline_mode() const;

	LinkButton(const String &p_text = String());
};

VARIANT_ENUM_CAST(LinkButton::UnderlineMode);

#endif // LINK_BUTTON_H
