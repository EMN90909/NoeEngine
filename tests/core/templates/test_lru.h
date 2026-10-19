// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef TEST_LRU_H
#define TEST_LRU_H

#include "core/templates/lru.h"

#include "tests/test_macros.h"

namespace TestLRU {

TEST_CASE("[LRU] Store and read") {
	LRUCache<int, int> lru;

	lru.set_capacity(3);
	lru.insert(1, 1);
	lru.insert(50, 2);
	lru.insert(100, 5);

	CHECK(lru.has(1));
	CHECK(lru.has(50));
	CHECK(lru.has(100));
	CHECK(!lru.has(200));

	CHECK(lru.get(1) == 1);
	CHECK(lru.get(50) == 2);
	CHECK(lru.get(100) == 5);

	CHECK(lru.getptr(1) != nullptr);
	CHECK(lru.getptr(1000) == nullptr);

	lru.insert(600, 600); // Erase <50>
	CHECK(lru.has(600));
	CHECK(!lru.has(50));
}

TEST_CASE("[LRU] Resize and clear") {
	LRUCache<int, int> lru;

	lru.set_capacity(3);
	lru.insert(1, 1);
	lru.insert(2, 2);
	lru.insert(3, 3);

	CHECK(lru.get_capacity() == 3);

	lru.set_capacity(5);
	CHECK(lru.get_capacity() == 5);

	CHECK(lru.has(1));
	CHECK(lru.has(2));
	CHECK(lru.has(3));
	CHECK(!lru.has(4));

	lru.set_capacity(2);
	CHECK(lru.get_capacity() == 2);

	CHECK(!lru.has(1));
	CHECK(lru.has(2));
	CHECK(lru.has(3));
	CHECK(!lru.has(4));

	lru.clear();
	CHECK(!lru.has(1));
	CHECK(!lru.has(2));
	CHECK(!lru.has(3));
	CHECK(!lru.has(4));
}
} // namespace TestLRU

#endif // TEST_LRU_H
