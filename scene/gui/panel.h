// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef PANEL_H
#define PANEL_H

#include "scene/gui/control.h"

class Panel : public Control {
	GDCLASS(Panel, Control);

	struct ThemeCache {
		Ref<StyleBox> panel_style;
	} theme_cache;

protected:
	virtual void _update_theme_item_cache() override;

	void _notification(int p_what);

public:
	Panel();
};

#endif // PANEL_H
