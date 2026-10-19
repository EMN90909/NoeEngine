// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef NAV_AGENT_H
#define NAV_AGENT_H

#include "core/object/class_db.h"
#include "nav_rid.h"

#include <Agent.h>

class NavMap;

class NavAgent : public NavRid {
	NavMap *map = nullptr;
	RVO::Agent agent;
	Callable callback = Callable();
	uint32_t map_update_id = 0;

public:
	void set_map(NavMap *p_map);
	NavMap *get_map() {
		return map;
	}

	RVO::Agent *get_agent() {
		return &agent;
	}

	bool is_map_changed();

	void set_callback(Callable p_callback);
	bool has_callback() const;

	void dispatch_callback();
};

#endif // NAV_AGENT_H
