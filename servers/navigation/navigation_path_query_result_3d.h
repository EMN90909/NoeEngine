// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef NAVIGATION_PATH_QUERY_RESULT_3D_H
#define NAVIGATION_PATH_QUERY_RESULT_3D_H

#include "core/object/ref_counted.h"
#include "core/variant/typed_array.h"
#include "servers/navigation/navigation_utilities.h"

class NavigationPathQueryResult3D : public RefCounted {
	GDCLASS(NavigationPathQueryResult3D, RefCounted);

	Vector<Vector3> path;
	Vector<int32_t> path_types;
	TypedArray<RID> path_rids;
	Vector<int64_t> path_owner_ids;

protected:
	static void _bind_methods();

public:
	enum PathSegmentType {
		PATH_SEGMENT_TYPE_REGION = 0,
		PATH_SEGMENT_TYPE_LINK = 1,
	};

	void set_path(const Vector<Vector3> &p_path);
	const Vector<Vector3> &get_path() const;

	void set_path_types(const Vector<int32_t> &p_path_types);
	const Vector<int32_t> &get_path_types() const;

	void set_path_rids(const TypedArray<RID> &p_path_rids);
	TypedArray<RID> get_path_rids() const;

	void set_path_owner_ids(const Vector<int64_t> &p_path_owner_ids);
	const Vector<int64_t> &get_path_owner_ids() const;

	void reset();
};

VARIANT_ENUM_CAST(NavigationPathQueryResult3D::PathSegmentType);

#endif // NAVIGATION_PATH_QUERY_RESULT_3D_H
