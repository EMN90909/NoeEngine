// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef VIEW_PANNER_H
#define VIEW_PANNER_H

#include "core/object/ref_counted.h"

class InputEvent;
class Shortcut;

class ViewPanner : public RefCounted {
	GDCLASS(ViewPanner, RefCounted);

public:
	enum ControlScheme {
		SCROLL_ZOOMS,
		SCROLL_PANS,
	};

	enum PanAxis {
		PAN_AXIS_BOTH,
		PAN_AXIS_HORIZONTAL,
		PAN_AXIS_VERTICAL,
	};

private:
	int scroll_speed = 32;
	float scroll_zoom_factor = 1.1;
	PanAxis pan_axis = PAN_AXIS_BOTH;

	bool is_dragging = false;
	bool pan_key_pressed = false;
	bool force_drag = false;

	bool enable_rmb = false;
	bool simple_panning_enabled = false;

	Ref<Shortcut> pan_view_shortcut;

	Callable pan_callback;
	Callable zoom_callback;

	void callback_helper(Callable p_callback, Vector<Variant> p_args);
	ControlScheme control_scheme = SCROLL_ZOOMS;

public:
	void set_callbacks(Callable p_pan_callback, Callable p_zoom_callback);
	void set_control_scheme(ControlScheme p_scheme);
	void set_enable_rmb(bool p_enable);
	void set_pan_shortcut(Ref<Shortcut> p_shortcut);
	void set_simple_panning_enabled(bool p_enabled);
	void set_scroll_speed(int p_scroll_speed);
	void set_scroll_zoom_factor(float p_scroll_zoom_factor);
	void set_pan_axis(PanAxis p_pan_axis);

	void setup(ControlScheme p_scheme, Ref<Shortcut> p_shortcut, bool p_simple_panning);

	bool is_panning() const;
	void set_force_drag(bool p_force);

	bool gui_input(const Ref<InputEvent> &p_ev, Rect2 p_canvas_rect = Rect2());
	void release_pan_key();

	ViewPanner();
};

#endif // VIEW_PANNER_H
