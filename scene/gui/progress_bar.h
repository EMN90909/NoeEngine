// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H

#include "scene/gui/range.h"

class ProgressBar : public Range {
	GDCLASS(ProgressBar, Range);

	bool show_percentage = true;

	struct ThemeCache {
		Ref<StyleBox> background_style;
		Ref<StyleBox> fill_style;

		Ref<Font> font;
		int font_size = 0;
		Color font_color;
		int font_outline_size = 0;
		Color font_outline_color;
	} theme_cache;

protected:
	virtual void _update_theme_item_cache() override;

	void _notification(int p_what);
	static void _bind_methods();

public:
	enum FillMode {
		FILL_BEGIN_TO_END,
		FILL_END_TO_BEGIN,
		FILL_TOP_TO_BOTTOM,
		FILL_BOTTOM_TO_TOP,
		FILL_MODE_MAX
	};

	void set_fill_mode(int p_fill);
	int get_fill_mode();

	void set_show_percentage(bool p_visible);
	bool is_percentage_shown() const;

	Size2 get_minimum_size() const override;
	ProgressBar();

private:
	FillMode mode = FILL_BEGIN_TO_END;
};

VARIANT_ENUM_CAST(ProgressBar::FillMode);

#endif // PROGRESS_BAR_H
