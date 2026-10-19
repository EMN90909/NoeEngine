// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "editor/editor_title_bar.h"

void EditorTitleBar::gui_input(const Ref<InputEvent> &p_event) {
	if (!can_move) {
		return;
	}

	Ref<InputEventMouseMotion> mm = p_event;
	if (mm.is_valid() && moving) {
		if (mm->get_button_mask().has_flag(MouseButtonMask::LEFT)) {
			Window *w = Object::cast_to<Window>(get_viewport());
			if (w) {
				Point2 mouse = DisplayServer::get_singleton()->mouse_get_position();
				w->set_position(mouse - click_pos);
			}
		} else {
			moving = false;
		}
	}

	Ref<InputEventMouseButton> mb = p_event;
	if (mb.is_valid() && has_point(mb->get_position())) {
		Window *w = Object::cast_to<Window>(get_viewport());
		if (w) {
			if (mb->get_button_index() == MouseButton::LEFT) {
				if (mb->is_pressed()) {
					click_pos = DisplayServer::get_singleton()->mouse_get_position() - w->get_position();
					moving = true;
				} else {
					moving = false;
				}
			}
			if (mb->get_button_index() == MouseButton::LEFT && mb->is_double_click() && mb->is_pressed()) {
				if (DisplayServer::get_singleton()->window_maximize_on_title_dbl_click()) {
					if (w->get_mode() == Window::MODE_WINDOWED) {
						w->set_mode(Window::MODE_MAXIMIZED);
					} else if (w->get_mode() == Window::MODE_MAXIMIZED) {
						w->set_mode(Window::MODE_WINDOWED);
					}
				} else if (DisplayServer::get_singleton()->window_minimize_on_title_dbl_click()) {
					w->set_mode(Window::MODE_MINIMIZED);
				}
				moving = false;
			}
		}
	}
}

void EditorTitleBar::set_can_move_window(bool p_enabled) {
	can_move = p_enabled;
	set_process_input(can_move);
}

bool EditorTitleBar::get_can_move_window() const {
	return can_move;
}
