// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "check_button.h"

#include "core/string/print_string.h"
#include "servers/rendering_server.h"

Size2 CheckButton::get_icon_size() const {
	Ref<Texture2D> on_tex;
	Ref<Texture2D> off_tex;

	if (is_layout_rtl()) {
		if (is_disabled()) {
			on_tex = theme_cache.checked_disabled_mirrored;
			off_tex = theme_cache.unchecked_disabled_mirrored;
		} else {
			on_tex = theme_cache.checked_mirrored;
			off_tex = theme_cache.unchecked_mirrored;
		}
	} else {
		if (is_disabled()) {
			on_tex = theme_cache.checked_disabled;
			off_tex = theme_cache.unchecked_disabled;
		} else {
			on_tex = theme_cache.checked;
			off_tex = theme_cache.unchecked;
		}
	}

	Size2 tex_size = Size2(0, 0);
	if (!on_tex.is_null()) {
		tex_size = Size2(on_tex->get_width(), on_tex->get_height());
	}
	if (!off_tex.is_null()) {
		tex_size = Size2(MAX(tex_size.width, off_tex->get_width()), MAX(tex_size.height, off_tex->get_height()));
	}

	return tex_size;
}

Size2 CheckButton::get_minimum_size() const {
	Size2 minsize = Button::get_minimum_size();
	Size2 tex_size = get_icon_size();
	minsize.width += tex_size.width;
	if (get_text().length() > 0) {
		minsize.width += MAX(0, theme_cache.h_separation);
	}
	minsize.height = MAX(minsize.height, tex_size.height + theme_cache.normal_style->get_margin(SIDE_TOP) + theme_cache.normal_style->get_margin(SIDE_BOTTOM));

	return minsize;
}

void CheckButton::_update_theme_item_cache() {
	Button::_update_theme_item_cache();

	theme_cache.h_separation = get_theme_constant(SNAME("h_separation"));
	theme_cache.check_v_offset = get_theme_constant(SNAME("check_v_offset"));
	theme_cache.normal_style = get_theme_stylebox(SNAME("normal"));

	theme_cache.checked = get_theme_icon(SNAME("checked"));
	theme_cache.unchecked = get_theme_icon(SNAME("unchecked"));
	theme_cache.checked_disabled = get_theme_icon(SNAME("checked_disabled"));
	theme_cache.unchecked_disabled = get_theme_icon(SNAME("unchecked_disabled"));
	theme_cache.checked_mirrored = get_theme_icon(SNAME("checked_mirrored"));
	theme_cache.unchecked_mirrored = get_theme_icon(SNAME("unchecked_mirrored"));
	theme_cache.checked_disabled_mirrored = get_theme_icon(SNAME("checked_disabled_mirrored"));
	theme_cache.unchecked_disabled_mirrored = get_theme_icon(SNAME("unchecked_disabled_mirrored"));
}

void CheckButton::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_THEME_CHANGED:
		case NOTIFICATION_LAYOUT_DIRECTION_CHANGED:
		case NOTIFICATION_TRANSLATION_CHANGED: {
			if (is_layout_rtl()) {
				_set_internal_margin(SIDE_LEFT, get_icon_size().width);
				_set_internal_margin(SIDE_RIGHT, 0.f);
			} else {
				_set_internal_margin(SIDE_LEFT, 0.f);
				_set_internal_margin(SIDE_RIGHT, get_icon_size().width);
			}
		} break;

		case NOTIFICATION_DRAW: {
			RID ci = get_canvas_item();
			bool rtl = is_layout_rtl();

			Ref<Texture2D> on_tex;
			Ref<Texture2D> off_tex;

			if (rtl) {
				if (is_disabled()) {
					on_tex = theme_cache.checked_disabled_mirrored;
					off_tex = theme_cache.unchecked_disabled_mirrored;
				} else {
					on_tex = theme_cache.checked_mirrored;
					off_tex = theme_cache.unchecked_mirrored;
				}
			} else {
				if (is_disabled()) {
					on_tex = theme_cache.checked_disabled;
					off_tex = theme_cache.unchecked_disabled;
				} else {
					on_tex = theme_cache.checked;
					off_tex = theme_cache.unchecked;
				}
			}

			Vector2 ofs;
			Size2 tex_size = get_icon_size();

			if (rtl) {
				ofs.x = theme_cache.normal_style->get_margin(SIDE_LEFT);
			} else {
				ofs.x = get_size().width - (tex_size.width + theme_cache.normal_style->get_margin(SIDE_RIGHT));
			}
			ofs.y = (get_size().height - tex_size.height) / 2 + theme_cache.check_v_offset;

			if (is_pressed()) {
				on_tex->draw(ci, ofs);
			} else {
				off_tex->draw(ci, ofs);
			}
		} break;
	}
}

CheckButton::CheckButton(const String &p_text) :
		Button(p_text) {
	set_toggle_mode(true);

	set_text_alignment(HORIZONTAL_ALIGNMENT_LEFT);

	if (is_layout_rtl()) {
		_set_internal_margin(SIDE_LEFT, get_icon_size().width);
	} else {
		_set_internal_margin(SIDE_RIGHT, get_icon_size().width);
	}
}

CheckButton::~CheckButton() {
}
