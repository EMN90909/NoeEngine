// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef NAV_REGION_H
#define NAV_REGION_H

#include "scene/resources/navigation_mesh.h"

#include "nav_base.h"
#include "nav_utils.h"

class NavRegion : public NavBase {
	NavMap *map = nullptr;
	Transform3D transform;
	Ref<NavigationMesh> mesh;
	Vector<gd::Edge::Connection> connections;

	bool polygons_dirty = true;

	/// Cache
	LocalVector<gd::Polygon> polygons;

public:
	NavRegion() {
		type = NavigationUtilities::PathSegmentType::PATH_SEGMENT_TYPE_REGION;
	}

	void scratch_polygons() {
		polygons_dirty = true;
	}

	void set_map(NavMap *p_map);
	NavMap *get_map() const {
		return map;
	}

	void set_transform(Transform3D transform);
	const Transform3D &get_transform() const {
		return transform;
	}

	void set_mesh(Ref<NavigationMesh> p_mesh);
	const Ref<NavigationMesh> get_mesh() const {
		return mesh;
	}

	Vector<gd::Edge::Connection> &get_connections() {
		return connections;
	}
	int get_connections_count() const;
	Vector3 get_connection_pathway_start(int p_connection_id) const;
	Vector3 get_connection_pathway_end(int p_connection_id) const;

	LocalVector<gd::Polygon> const &get_polygons() const {
		return polygons;
	}

	bool sync();

private:
	void update_polygons();
};

#endif // NAV_REGION_H
