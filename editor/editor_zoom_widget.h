// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef EDITOR_ZOOM_WIDGET_H
#define EDITOR_ZOOM_WIDGET_H

#include "scene/gui/box_container.h"
#include "scene/gui/button.h"

class EditorZoomWidget : public HBoxContainer {
	GDCLASS(EditorZoomWidget, HBoxContainer);

	Button *zoom_minus = nullptr;
	Button *zoom_reset = nullptr;
	Button *zoom_plus = nullptr;

	float zoom = 1.0;
	void _update_zoom_label();
	void _button_zoom_minus();
	void _button_zoom_reset();
	void _button_zoom_plus();

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	EditorZoomWidget();

	float get_zoom();
	void set_zoom(float p_zoom);
	void set_zoom_by_increments(int p_increment_count, bool p_integer_only = false);
	// Sets the shortcut context for the zoom buttons. By default their context is this EditorZoomWidget control.
	void set_shortcut_context(Node *p_node) const;
};

#endif // EDITOR_ZOOM_WIDGET_H
