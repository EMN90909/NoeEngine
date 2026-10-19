// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef MARKER_2D_H
#define MARKER_2D_H

#include "scene/2d/node_2d.h"

class Marker2D : public Node2D {
	GDCLASS(Marker2D, Node2D);

	real_t gizmo_extents = 10.0;

	void _draw_cross();

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
#ifdef TOOLS_ENABLED
	virtual Rect2 _edit_get_rect() const override;
	virtual bool _edit_use_rect() const override;
#endif

	void set_gizmo_extents(real_t p_extents);
	real_t get_gizmo_extents() const;

	Marker2D();
};

#endif // MARKER_2D_H
