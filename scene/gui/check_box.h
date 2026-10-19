// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef CHECK_BOX_H
#define CHECK_BOX_H

#include "scene/gui/button.h"

class CheckBox : public Button {
	GDCLASS(CheckBox, Button);

	struct ThemeCache {
		int h_separation = 0;
		int check_v_offset = 0;
		Ref<StyleBox> normal_style;

		Ref<Texture2D> checked;
		Ref<Texture2D> unchecked;
		Ref<Texture2D> radio_checked;
		Ref<Texture2D> radio_unchecked;
		Ref<Texture2D> checked_disabled;
		Ref<Texture2D> unchecked_disabled;
		Ref<Texture2D> radio_checked_disabled;
		Ref<Texture2D> radio_unchecked_disabled;
	} theme_cache;

protected:
	Size2 get_icon_size() const;
	Size2 get_minimum_size() const override;

	virtual void _update_theme_item_cache() override;
	void _notification(int p_what);

	bool is_radio();

public:
	CheckBox(const String &p_text = String());
	~CheckBox();
};

#endif // CHECK_BOX_H
