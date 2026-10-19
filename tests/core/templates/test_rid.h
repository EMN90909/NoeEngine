// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef TEST_RID_H
#define TEST_RID_H

#include "core/templates/rid.h"

#include "tests/test_macros.h"

namespace TestRID {
TEST_CASE("[RID] Default Constructor") {
	RID rid;

	CHECK(rid.get_id() == 0);
}

TEST_CASE("[RID] Factory method") {
	RID rid = RID::from_uint64(1);

	CHECK(rid.get_id() == 1);
}

TEST_CASE("[RID] Operators") {
	RID rid = RID::from_uint64(1);

	RID rid_zero = RID::from_uint64(0);
	RID rid_one = RID::from_uint64(1);
	RID rid_two = RID::from_uint64(2);

	CHECK_FALSE(rid == rid_zero);
	CHECK(rid == rid_one);
	CHECK_FALSE(rid == rid_two);

	CHECK_FALSE(rid < rid_zero);
	CHECK_FALSE(rid < rid_one);
	CHECK(rid < rid_two);

	CHECK_FALSE(rid <= rid_zero);
	CHECK(rid <= rid_one);
	CHECK(rid <= rid_two);

	CHECK(rid > rid_zero);
	CHECK_FALSE(rid > rid_one);
	CHECK_FALSE(rid > rid_two);

	CHECK(rid >= rid_zero);
	CHECK(rid >= rid_one);
	CHECK_FALSE(rid >= rid_two);

	CHECK(rid != rid_zero);
	CHECK_FALSE(rid != rid_one);
	CHECK(rid != rid_two);
}

TEST_CASE("[RID] 'is_valid' & 'is_null'") {
	RID rid_zero = RID::from_uint64(0);
	RID rid_one = RID::from_uint64(1);

	CHECK_FALSE(rid_zero.is_valid());
	CHECK(rid_zero.is_null());

	CHECK(rid_one.is_valid());
	CHECK_FALSE(rid_one.is_null());
}

TEST_CASE("[RID] 'get_local_index'") {
	CHECK(RID::from_uint64(1).get_local_index() == 1);
	CHECK(RID::from_uint64(4'294'967'295).get_local_index() == 4'294'967'295);
	CHECK(RID::from_uint64(4'294'967'297).get_local_index() == 1);
}
} // namespace TestRID

#endif // TEST_RID_H
