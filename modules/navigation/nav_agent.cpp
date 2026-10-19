// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "nav_agent.h"

#include "nav_map.h"

void NavAgent::set_map(NavMap *p_map) {
	map = p_map;
}

bool NavAgent::is_map_changed() {
	if (map) {
		bool is_changed = map->get_map_update_id() != map_update_id;
		map_update_id = map->get_map_update_id();
		return is_changed;
	} else {
		return false;
	}
}

void NavAgent::set_callback(Callable p_callback) {
	callback = p_callback;
}

bool NavAgent::has_callback() const {
	return callback.is_valid();
}

void NavAgent::dispatch_callback() {
	if (!callback.is_valid()) {
		return;
	}

	Vector3 new_velocity = Vector3(agent.newVelocity_.x(), agent.newVelocity_.y(), agent.newVelocity_.z());

	// Invoke the callback with the new velocity.
	Variant args[] = { new_velocity };
	const Variant *args_p[] = { &args[0] };
	Variant return_value;
	Callable::CallError call_error;
	callback.callp(args_p, 1, return_value, call_error);
}
