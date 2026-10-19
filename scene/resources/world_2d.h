// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef WORLD_2D_H
#define WORLD_2D_H

#include "core/io/resource.h"
#include "scene/resources/world_2d.h"
#include "servers/physics_server_2d.h"

class VisibleOnScreenNotifier2D;
class Viewport;
struct SpatialIndexer2D;

class World2D : public Resource {
	GDCLASS(World2D, Resource);

	RID canvas;
	mutable RID space;
	mutable RID navigation_map;

	HashSet<Viewport *> viewports;

protected:
	static void _bind_methods();
	friend class Viewport;

public:
	RID get_canvas() const;
	RID get_space() const;
	RID get_navigation_map() const;

	PhysicsDirectSpaceState2D *get_direct_space_state();

	void register_viewport(Viewport *p_viewport);
	void remove_viewport(Viewport *p_viewport);

	_FORCE_INLINE_ const HashSet<Viewport *> &get_viewports() { return viewports; }

	World2D();
	~World2D();
};

#endif // WORLD_2D_H
