// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "nav_link.h"

#include "nav_map.h"

void NavLink::set_map(NavMap *p_map) {
	map = p_map;
	link_dirty = true;
}

void NavLink::set_bidirectional(bool p_bidirectional) {
	bidirectional = p_bidirectional;
	link_dirty = true;
}

void NavLink::set_start_position(const Vector3 p_position) {
	start_position = p_position;
	link_dirty = true;
}

void NavLink::set_end_position(const Vector3 p_position) {
	end_position = p_position;
	link_dirty = true;
}

bool NavLink::check_dirty() {
	const bool was_dirty = link_dirty;

	link_dirty = false;
	return was_dirty;
}
