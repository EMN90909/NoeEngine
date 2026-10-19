// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef GRID_CONTAINER_H
#define GRID_CONTAINER_H

#include "scene/gui/container.h"

class GridContainer : public Container {
	GDCLASS(GridContainer, Container);

	int columns = 1;

	struct ThemeCache {
		int h_separation = 0;
		int v_separation = 0;
	} theme_cache;

protected:
	virtual void _update_theme_item_cache() override;

	void _notification(int p_what);
	static void _bind_methods();

public:
	void set_columns(int p_columns);
	int get_columns() const;
	virtual Size2 get_minimum_size() const override;

	GridContainer();
};

#endif // GRID_CONTAINER_H
