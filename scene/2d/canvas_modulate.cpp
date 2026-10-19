// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "canvas_modulate.h"

void CanvasModulate::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_CANVAS: {
			if (is_visible_in_tree()) {
				RS::get_singleton()->canvas_set_modulate(get_canvas(), color);
				add_to_group("_canvas_modulate_" + itos(get_canvas().get_id()));
			}
		} break;

		case NOTIFICATION_EXIT_CANVAS: {
			if (is_visible_in_tree()) {
				RS::get_singleton()->canvas_set_modulate(get_canvas(), Color(1, 1, 1, 1));
				remove_from_group("_canvas_modulate_" + itos(get_canvas().get_id()));
			}
		} break;

		case NOTIFICATION_VISIBILITY_CHANGED: {
			if (is_visible_in_tree()) {
				RS::get_singleton()->canvas_set_modulate(get_canvas(), color);
				add_to_group("_canvas_modulate_" + itos(get_canvas().get_id()));
			} else {
				RS::get_singleton()->canvas_set_modulate(get_canvas(), Color(1, 1, 1, 1));
				remove_from_group("_canvas_modulate_" + itos(get_canvas().get_id()));
			}

			update_configuration_warnings();
		} break;
	}
}

void CanvasModulate::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_color", "color"), &CanvasModulate::set_color);
	ClassDB::bind_method(D_METHOD("get_color"), &CanvasModulate::get_color);

	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "color"), "set_color", "get_color");
}

void CanvasModulate::set_color(const Color &p_color) {
	color = p_color;
	if (is_visible_in_tree()) {
		RS::get_singleton()->canvas_set_modulate(get_canvas(), color);
	}
}

Color CanvasModulate::get_color() const {
	return color;
}

PackedStringArray CanvasModulate::get_configuration_warnings() const {
	PackedStringArray warnings = Node::get_configuration_warnings();

	if (is_visible_in_tree() && is_inside_tree()) {
		List<Node *> nodes;
		get_tree()->get_nodes_in_group("_canvas_modulate_" + itos(get_canvas().get_id()), &nodes);

		if (nodes.size() > 1) {
			warnings.push_back(RTR("Only one visible CanvasModulate is allowed per scene (or set of instantiated scenes). The first created one will work, while the rest will be ignored."));
		}
	}

	return warnings;
}

CanvasModulate::CanvasModulate() {
}

CanvasModulate::~CanvasModulate() {
}
