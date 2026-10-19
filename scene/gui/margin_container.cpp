// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "margin_container.h"

void MarginContainer::_update_theme_item_cache() {
	Container::_update_theme_item_cache();

	theme_cache.margin_left = get_theme_constant(SNAME("margin_left"));
	theme_cache.margin_top = get_theme_constant(SNAME("margin_top"));
	theme_cache.margin_right = get_theme_constant(SNAME("margin_right"));
	theme_cache.margin_bottom = get_theme_constant(SNAME("margin_bottom"));
}

Size2 MarginContainer::get_minimum_size() const {
	Size2 max;

	for (int i = 0; i < get_child_count(); i++) {
		Control *c = Object::cast_to<Control>(get_child(i));
		if (!c) {
			continue;
		}
		if (c->is_set_as_top_level()) {
			continue;
		}
		if (!c->is_visible()) {
			continue;
		}

		Size2 s = c->get_combined_minimum_size();
		if (s.width > max.width) {
			max.width = s.width;
		}
		if (s.height > max.height) {
			max.height = s.height;
		}
	}

	max.width += (theme_cache.margin_left + theme_cache.margin_right);
	max.height += (theme_cache.margin_top + theme_cache.margin_bottom);

	return max;
}

Vector<int> MarginContainer::get_allowed_size_flags_horizontal() const {
	Vector<int> flags;
	flags.append(SIZE_FILL);
	flags.append(SIZE_SHRINK_BEGIN);
	flags.append(SIZE_SHRINK_CENTER);
	flags.append(SIZE_SHRINK_END);
	return flags;
}

Vector<int> MarginContainer::get_allowed_size_flags_vertical() const {
	Vector<int> flags;
	flags.append(SIZE_FILL);
	flags.append(SIZE_SHRINK_BEGIN);
	flags.append(SIZE_SHRINK_CENTER);
	flags.append(SIZE_SHRINK_END);
	return flags;
}

void MarginContainer::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_SORT_CHILDREN: {
			Size2 s = get_size();

			for (int i = 0; i < get_child_count(); i++) {
				Control *c = Object::cast_to<Control>(get_child(i));
				if (!c) {
					continue;
				}
				if (c->is_set_as_top_level()) {
					continue;
				}

				int w = s.width - theme_cache.margin_left - theme_cache.margin_right;
				int h = s.height - theme_cache.margin_top - theme_cache.margin_bottom;
				fit_child_in_rect(c, Rect2(theme_cache.margin_left, theme_cache.margin_top, w, h));
			}
		} break;

		case NOTIFICATION_THEME_CHANGED: {
			update_minimum_size();
		} break;
	}
}

MarginContainer::MarginContainer() {
}
