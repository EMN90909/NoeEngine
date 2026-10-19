// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef EDITOR_TITLE_BAR_H
#define EDITOR_TITLE_BAR_H

#include "scene/gui/box_container.h"
#include "scene/main/window.h"

class EditorTitleBar : public HBoxContainer {
	GDCLASS(EditorTitleBar, HBoxContainer);

	Point2i click_pos;
	bool moving = false;
	bool can_move = false;

protected:
	virtual void gui_input(const Ref<InputEvent> &p_event) override;
	static void _bind_methods() {};

public:
	void set_can_move_window(bool p_enabled);
	bool get_can_move_window() const;
};

#endif // EDITOR_TITLE_BAR_H
