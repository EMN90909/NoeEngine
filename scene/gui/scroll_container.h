// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef SCROLL_CONTAINER_H
#define SCROLL_CONTAINER_H

#include "container.h"

#include "scroll_bar.h"

class ScrollContainer : public Container {
	GDCLASS(ScrollContainer, Container);

public:
	enum ScrollMode {
		SCROLL_MODE_DISABLED = 0,
		SCROLL_MODE_AUTO,
		SCROLL_MODE_SHOW_ALWAYS,
		SCROLL_MODE_SHOW_NEVER,
	};

private:
	HScrollBar *h_scroll = nullptr;
	VScrollBar *v_scroll = nullptr;

	mutable Size2 largest_child_min_size; // The largest one among the min sizes of all available child controls.

	void update_scrollbars();

	Vector2 drag_speed;
	Vector2 drag_accum;
	Vector2 drag_from;
	Vector2 last_drag_accum;
	float time_since_motion = 0.0f;
	bool drag_touching = false;
	bool drag_touching_deaccel = false;
	bool beyond_deadzone = false;

	ScrollMode horizontal_scroll_mode = SCROLL_MODE_AUTO;
	ScrollMode vertical_scroll_mode = SCROLL_MODE_AUTO;

	int deadzone = 0;
	bool follow_focus = false;

	struct ThemeCache {
		Ref<StyleBox> panel_style;
	} theme_cache;

	void _cancel_drag();

protected:
	virtual void _update_theme_item_cache() override;
	Size2 get_minimum_size() const override;

	void _gui_focus_changed(Control *p_control);
	void _reposition_children();
	void _notification(int p_what);

	void _scroll_moved(float);
	static void _bind_methods();

	bool _updating_scrollbars = false;
	void _update_scrollbar_position();

public:
	virtual void gui_input(const Ref<InputEvent> &p_gui_input) override;

	void set_h_scroll(int p_pos);
	int get_h_scroll() const;

	void set_v_scroll(int p_pos);
	int get_v_scroll() const;

	void set_horizontal_scroll_mode(ScrollMode p_mode);
	ScrollMode get_horizontal_scroll_mode() const;

	void set_vertical_scroll_mode(ScrollMode p_mode);
	ScrollMode get_vertical_scroll_mode() const;

	int get_deadzone() const;
	void set_deadzone(int p_deadzone);

	bool is_following_focus() const;
	void set_follow_focus(bool p_follow);

	HScrollBar *get_h_scroll_bar();
	VScrollBar *get_v_scroll_bar();
	void ensure_control_visible(Control *p_control);

	PackedStringArray get_configuration_warnings() const override;

	ScrollContainer();
};

VARIANT_ENUM_CAST(ScrollContainer::ScrollMode);

#endif // SCROLL_CONTAINER_H
