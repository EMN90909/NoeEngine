// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef TEST_HASH_MAP_H
#define TEST_HASH_MAP_H

#include "core/templates/hash_map.h"

#include "tests/test_macros.h"

namespace TestHashMap {

TEST_CASE("[HashMap] Insert element") {
	HashMap<int, int> map;
	HashMap<int, int>::Iterator e = map.insert(42, 84);

	CHECK(e);
	CHECK(e->key == 42);
	CHECK(e->value == 84);
	CHECK(map[42] == 84);
	CHECK(map.has(42));
	CHECK(map.find(42));
}

TEST_CASE("[HashMap] Overwrite element") {
	HashMap<int, int> map;
	map.insert(42, 84);
	map.insert(42, 1234);

	CHECK(map[42] == 1234);
}

TEST_CASE("[HashMap] Erase via element") {
	HashMap<int, int> map;
	HashMap<int, int>::Iterator e = map.insert(42, 84);
	map.remove(e);
	CHECK(!map.has(42));
	CHECK(!map.find(42));
}

TEST_CASE("[HashMap] Erase via key") {
	HashMap<int, int> map;
	map.insert(42, 84);
	map.erase(42);
	CHECK(!map.has(42));
	CHECK(!map.find(42));
}

TEST_CASE("[HashMap] Size") {
	HashMap<int, int> map;
	map.insert(42, 84);
	map.insert(123, 84);
	map.insert(123, 84);
	map.insert(0, 84);
	map.insert(123485, 84);

	CHECK(map.size() == 4);
}

TEST_CASE("[HashMap] Iteration") {
	HashMap<int, int> map;
	map.insert(42, 84);
	map.insert(123, 12385);
	map.insert(0, 12934);
	map.insert(123485, 1238888);
	map.insert(123, 111111);

	Vector<Pair<int, int>> expected;
	expected.push_back(Pair<int, int>(42, 84));
	expected.push_back(Pair<int, int>(123, 111111));
	expected.push_back(Pair<int, int>(0, 12934));
	expected.push_back(Pair<int, int>(123485, 1238888));

	int idx = 0;
	for (const KeyValue<int, int> &E : map) {
		CHECK(expected[idx] == Pair<int, int>(E.key, E.value));
		++idx;
	}
}

TEST_CASE("[HashMap] Const iteration") {
	HashMap<int, int> map;
	map.insert(42, 84);
	map.insert(123, 12385);
	map.insert(0, 12934);
	map.insert(123485, 1238888);
	map.insert(123, 111111);

	const HashMap<int, int> const_map = map;

	Vector<Pair<int, int>> expected;
	expected.push_back(Pair<int, int>(42, 84));
	expected.push_back(Pair<int, int>(123, 111111));
	expected.push_back(Pair<int, int>(0, 12934));
	expected.push_back(Pair<int, int>(123485, 1238888));
	expected.push_back(Pair<int, int>(123, 111111));

	int idx = 0;
	for (const KeyValue<int, int> &E : const_map) {
		CHECK(expected[idx] == Pair<int, int>(E.key, E.value));
		++idx;
	}
}
} // namespace TestHashMap

#endif // TEST_HASH_MAP_H
