// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef NAVIGATION_REGION_2D_H
#define NAVIGATION_REGION_2D_H

#include "scene/resources/navigation_polygon.h"

class NavigationRegion2D : public Node2D {
	GDCLASS(NavigationRegion2D, Node2D);

	bool enabled = true;
	RID region;
	uint32_t navigation_layers = 1;
	real_t enter_cost = 0.0;
	real_t travel_cost = 1.0;
	Ref<NavigationPolygon> navigation_polygon;

	void _navigation_polygon_changed();
	void _map_changed(RID p_RID);

protected:
	void _notification(int p_what);
	static void _bind_methods();

#ifndef DISABLE_DEPRECATED
	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
#endif // DISABLE_DEPRECATED

public:
#ifdef TOOLS_ENABLED
	virtual Rect2 _edit_get_rect() const override;
	virtual bool _edit_is_selected_on_click(const Point2 &p_point, double p_tolerance) const override;
#endif

	void set_enabled(bool p_enabled);
	bool is_enabled() const;

	void set_navigation_layers(uint32_t p_navigation_layers);
	uint32_t get_navigation_layers() const;

	void set_navigation_layer_value(int p_layer_number, bool p_value);
	bool get_navigation_layer_value(int p_layer_number) const;

	RID get_region_rid() const;

	void set_enter_cost(real_t p_enter_cost);
	real_t get_enter_cost() const;

	void set_travel_cost(real_t p_travel_cost);
	real_t get_travel_cost() const;

	void set_navigation_polygon(const Ref<NavigationPolygon> &p_navigation_polygon);
	Ref<NavigationPolygon> get_navigation_polygon() const;

	PackedStringArray get_configuration_warnings() const override;

	NavigationRegion2D();
	~NavigationRegion2D();
};

#endif // NAVIGATION_REGION_2D_H
