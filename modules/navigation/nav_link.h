// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef NAV_LINK_H
#define NAV_LINK_H

#include "nav_base.h"
#include "nav_utils.h"

class NavLink : public NavBase {
	NavMap *map = nullptr;
	bool bidirectional = true;
	Vector3 start_position;
	Vector3 end_position;

	bool link_dirty = true;

public:
	NavLink() {
		type = NavigationUtilities::PathSegmentType::PATH_SEGMENT_TYPE_LINK;
	}

	void set_map(NavMap *p_map);
	NavMap *get_map() const {
		return map;
	}

	void set_bidirectional(bool p_bidirectional);
	bool is_bidirectional() const {
		return bidirectional;
	}

	void set_start_position(Vector3 p_position);
	Vector3 get_start_position() const {
		return start_position;
	}

	void set_end_position(Vector3 p_position);
	Vector3 get_end_position() const {
		return end_position;
	}

	bool check_dirty();
};

#endif // NAV_LINK_H
