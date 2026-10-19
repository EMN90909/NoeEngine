// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef NAV_UTILS_H
#define NAV_UTILS_H

#include "core/math/vector3.h"
#include "core/templates/hash_map.h"
#include "core/templates/hashfuncs.h"
#include "core/templates/local_vector.h"

class NavBase;

namespace gd {
struct Polygon;

union PointKey {
	struct {
		int64_t x : 21;
		int64_t y : 22;
		int64_t z : 21;
	};

	uint64_t key = 0;
};

struct EdgeKey {
	PointKey a;
	PointKey b;

	static uint32_t hash(const EdgeKey &p_val) {
		return hash_one_uint64(p_val.a.key) ^ hash_one_uint64(p_val.b.key);
	}

	bool operator==(const EdgeKey &p_key) const {
		return (a.key == p_key.a.key) && (b.key == p_key.b.key);
	}

	EdgeKey(const PointKey &p_a = PointKey(), const PointKey &p_b = PointKey()) :
			a(p_a),
			b(p_b) {
		if (a.key > b.key) {
			SWAP(a, b);
		}
	}
};

struct Point {
	Vector3 pos;
	PointKey key;
};

struct Edge {
	/// The gateway in the edge, as, in some case, the whole edge might not be navigable.
	struct Connection {
		/// Polygon that this connection leads to.
		Polygon *polygon = nullptr;

		/// Edge of the source polygon where this connection starts from.
		int edge = -1;

		/// Point on the edge where the gateway leading to the poly starts.
		Vector3 pathway_start;

		/// Point on the edge where the gateway leading to the poly ends.
		Vector3 pathway_end;
	};

	/// Connections from this edge to other polygons.
	Vector<Connection> connections;
};

struct Polygon {
	/// Navigation region or link that contains this polygon.
	const NavBase *owner = nullptr;

	/// The points of this `Polygon`
	LocalVector<Point> points;

	/// Are the points clockwise?
	bool clockwise;

	/// The edges of this `Polygon`
	LocalVector<Edge> edges;

	/// The center of this `Polygon`
	Vector3 center;
};

struct NavigationPoly {
	uint32_t self_id = 0;
	/// This poly.
	const Polygon *poly;

	/// Those 4 variables are used to travel the path backwards.
	int back_navigation_poly_id = -1;
	int back_navigation_edge = -1;
	Vector3 back_navigation_edge_pathway_start;
	Vector3 back_navigation_edge_pathway_end;

	/// The entry position of this poly.
	Vector3 entry;
	/// The distance to the destination.
	float traveled_distance = 0.0;

	NavigationPoly() { poly = nullptr; }

	NavigationPoly(const Polygon *p_poly) :
			poly(p_poly) {}

	bool operator==(const NavigationPoly &other) const {
		return this->poly == other.poly;
	}

	bool operator!=(const NavigationPoly &other) const {
		return !operator==(other);
	}
};

struct ClosestPointQueryResult {
	Vector3 point;
	Vector3 normal;
	RID owner;
};

} // namespace gd

#endif // NAV_UTILS_H
