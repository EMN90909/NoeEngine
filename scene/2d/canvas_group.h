// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef CANVAS_GROUP_H
#define CANVAS_GROUP_H

#include "scene/2d/node_2d.h"

class CanvasGroup : public Node2D {
	GDCLASS(CanvasGroup, Node2D)
	real_t fit_margin = 10.0;
	real_t clear_margin = 10.0;
	bool use_mipmaps = false;

protected:
	static void _bind_methods();

public:
	void set_fit_margin(real_t p_fit_margin);
	real_t get_fit_margin() const;

	void set_clear_margin(real_t p_clear_margin);
	real_t get_clear_margin() const;

	void set_use_mipmaps(bool p_use_mipmaps);
	bool is_using_mipmaps() const;

	CanvasGroup();
	~CanvasGroup();
};

#endif // CANVAS_GROUP_H
