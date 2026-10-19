// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef MARGIN_CONTAINER_H
#define MARGIN_CONTAINER_H

#include "scene/gui/container.h"

class MarginContainer : public Container {
	GDCLASS(MarginContainer, Container);

	struct ThemeCache {
		int margin_left = 0;
		int margin_top = 0;
		int margin_right = 0;
		int margin_bottom = 0;
	} theme_cache;

protected:
	virtual void _update_theme_item_cache() override;

	void _notification(int p_what);

public:
	virtual Size2 get_minimum_size() const override;

	virtual Vector<int> get_allowed_size_flags_horizontal() const override;
	virtual Vector<int> get_allowed_size_flags_vertical() const override;

	MarginContainer();
};

#endif // MARGIN_CONTAINER_H
