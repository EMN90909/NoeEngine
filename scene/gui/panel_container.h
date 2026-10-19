// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef PANEL_CONTAINER_H
#define PANEL_CONTAINER_H

#include "scene/gui/container.h"

class PanelContainer : public Container {
	GDCLASS(PanelContainer, Container);

	struct ThemeCache {
		Ref<StyleBox> panel_style;
	} theme_cache;

protected:
	virtual void _update_theme_item_cache() override;
	void _notification(int p_what);

public:
	virtual Size2 get_minimum_size() const override;

	virtual Vector<int> get_allowed_size_flags_horizontal() const override;
	virtual Vector<int> get_allowed_size_flags_vertical() const override;

	PanelContainer();
};

#endif // PANEL_CONTAINER_H
