// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef POPUP_H
#define POPUP_H

#include "scene/main/window.h"

#include "core/templates/local_vector.h"

class Panel;

class Popup : public Window {
	GDCLASS(Popup, Window);

	LocalVector<Window *> visible_parents;
	bool popped_up = false;

	struct ThemeCache {
		Ref<StyleBox> panel_style;
	} theme_cache;

	void _input_from_window(const Ref<InputEvent> &p_event);

	void _initialize_visible_parents();
	void _deinitialize_visible_parents();

protected:
	void _close_pressed();
	virtual Rect2i _popup_adjust_rect() const override;

	virtual void _update_theme_item_cache() override;
	void _notification(int p_what);
	static void _bind_methods();
	void _validate_property(PropertyInfo &p_property) const;

	virtual void _parent_focused();

	virtual void _post_popup() override;

public:
	Popup();
	~Popup();
};

class PopupPanel : public Popup {
	GDCLASS(PopupPanel, Popup);

	Panel *panel = nullptr;

	struct ThemeCache {
		Ref<StyleBox> panel_style;
	} theme_cache;

protected:
	void _update_child_rects();

	virtual void _update_theme_item_cache() override;
	void _notification(int p_what);

	virtual Size2 _get_contents_minimum_size() const override;

public:
	PopupPanel();
};

#endif // POPUP_H
