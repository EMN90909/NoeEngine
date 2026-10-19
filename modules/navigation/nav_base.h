// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef NAV_BASE_H
#define NAV_BASE_H

#include "nav_rid.h"
#include "nav_utils.h"
#include "servers/navigation/navigation_utilities.h"

class NavMap;

class NavBase : public NavRid {
protected:
	uint32_t navigation_layers = 1;
	float enter_cost = 0.0;
	float travel_cost = 1.0;
	ObjectID owner_id;
	NavigationUtilities::PathSegmentType type;

public:
	NavigationUtilities::PathSegmentType get_type() const { return type; }

	void set_navigation_layers(uint32_t p_navigation_layers) { navigation_layers = p_navigation_layers; }
	uint32_t get_navigation_layers() const { return navigation_layers; }

	void set_enter_cost(float p_enter_cost) { enter_cost = MAX(p_enter_cost, 0.0); }
	float get_enter_cost() const { return enter_cost; }

	void set_travel_cost(float p_travel_cost) { travel_cost = MAX(p_travel_cost, 0.0); }
	float get_travel_cost() const { return travel_cost; }

	void set_owner_id(ObjectID p_owner_id) { owner_id = p_owner_id; }
	ObjectID get_owner_id() const { return owner_id; }
};

#endif // NAV_BASE_H
