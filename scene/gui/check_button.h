// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef CHECK_BUTTON_H
#define CHECK_BUTTON_H

#include "scene/gui/button.h"

class CheckButton : public Button {
	GDCLASS(CheckButton, Button);

	struct ThemeCache {
		int h_separation = 0;
		int check_v_offset = 0;
		Ref<StyleBox> normal_style;

		Ref<Texture2D> checked;
		Ref<Texture2D> unchecked;
		Ref<Texture2D> checked_disabled;
		Ref<Texture2D> unchecked_disabled;
		Ref<Texture2D> checked_mirrored;
		Ref<Texture2D> unchecked_mirrored;
		Ref<Texture2D> checked_disabled_mirrored;
		Ref<Texture2D> unchecked_disabled_mirrored;
	} theme_cache;

protected:
	Size2 get_icon_size() const;
	virtual Size2 get_minimum_size() const override;

	virtual void _update_theme_item_cache() override;
	void _notification(int p_what);

public:
	CheckButton(const String &p_text = String());
	~CheckButton();
};

#endif // CHECK_BUTTON_H
