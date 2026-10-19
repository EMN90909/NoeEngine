// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "navigation_path_query_result_2d.h"

void NavigationPathQueryResult2D::set_path(const Vector<Vector2> &p_path) {
	path = p_path;
}

const Vector<Vector2> &NavigationPathQueryResult2D::get_path() const {
	return path;
}

void NavigationPathQueryResult2D::set_path_types(const Vector<int32_t> &p_path_types) {
	path_types = p_path_types;
}

const Vector<int32_t> &NavigationPathQueryResult2D::get_path_types() const {
	return path_types;
}

void NavigationPathQueryResult2D::set_path_rids(const TypedArray<RID> &p_path_rids) {
	path_rids = p_path_rids;
}

TypedArray<RID> NavigationPathQueryResult2D::get_path_rids() const {
	return path_rids;
}

void NavigationPathQueryResult2D::set_path_owner_ids(const Vector<int64_t> &p_path_owner_ids) {
	path_owner_ids = p_path_owner_ids;
}

const Vector<int64_t> &NavigationPathQueryResult2D::get_path_owner_ids() const {
	return path_owner_ids;
}

void NavigationPathQueryResult2D::reset() {
	path.clear();
	path_types.clear();
	path_rids.clear();
	path_owner_ids.clear();
}

void NavigationPathQueryResult2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_path", "path"), &NavigationPathQueryResult2D::set_path);
	ClassDB::bind_method(D_METHOD("get_path"), &NavigationPathQueryResult2D::get_path);

	ClassDB::bind_method(D_METHOD("set_path_types", "path_types"), &NavigationPathQueryResult2D::set_path_types);
	ClassDB::bind_method(D_METHOD("get_path_types"), &NavigationPathQueryResult2D::get_path_types);

	ClassDB::bind_method(D_METHOD("set_path_rids", "path_rids"), &NavigationPathQueryResult2D::set_path_rids);
	ClassDB::bind_method(D_METHOD("get_path_rids"), &NavigationPathQueryResult2D::get_path_rids);

	ClassDB::bind_method(D_METHOD("set_path_owner_ids", "path_owner_ids"), &NavigationPathQueryResult2D::set_path_owner_ids);
	ClassDB::bind_method(D_METHOD("get_path_owner_ids"), &NavigationPathQueryResult2D::get_path_owner_ids);

	ClassDB::bind_method(D_METHOD("reset"), &NavigationPathQueryResult2D::reset);

	ADD_PROPERTY(PropertyInfo(Variant::PACKED_VECTOR2_ARRAY, "path"), "set_path", "get_path");
	ADD_PROPERTY(PropertyInfo(Variant::PACKED_INT32_ARRAY, "path_types"), "set_path_types", "get_path_types");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "path_rids", PROPERTY_HINT_ARRAY_TYPE, "RID"), "set_path_rids", "get_path_rids");
	ADD_PROPERTY(PropertyInfo(Variant::PACKED_INT64_ARRAY, "path_owner_ids"), "set_path_owner_ids", "get_path_owner_ids");

	BIND_ENUM_CONSTANT(PATH_SEGMENT_TYPE_REGION);
	BIND_ENUM_CONSTANT(PATH_SEGMENT_TYPE_LINK);
}
