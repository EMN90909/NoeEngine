// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef TEST_VECTOR3I_H
#define TEST_VECTOR3I_H

#include "core/math/vector3i.h"
#include "tests/test_macros.h"

namespace TestVector3i {

TEST_CASE("[Vector3i] Constructor methods") {
	const Vector3i vector_empty = Vector3i();
	const Vector3i vector_zero = Vector3i(0, 0, 0);
	CHECK_MESSAGE(
			vector_empty == vector_zero,
			"Vector3i Constructor with no inputs should return a zero Vector3i.");
}

TEST_CASE("[Vector3i] Axis methods") {
	Vector3i vector = Vector3i(1, 2, 3);
	CHECK_MESSAGE(
			vector.max_axis_index() == Vector3i::Axis::AXIS_Z,
			"Vector3i max_axis_index should work as expected.");
	CHECK_MESSAGE(
			vector.min_axis_index() == Vector3i::Axis::AXIS_X,
			"Vector3i min_axis_index should work as expected.");
	CHECK_MESSAGE(
			vector[vector.max_axis_index()] == 3,
			"Vector3i array operator should work as expected.");
	CHECK_MESSAGE(
			vector[vector.min_axis_index()] == 1,
			"Vector3i array operator should work as expected.");

	vector[Vector3i::Axis::AXIS_Y] = 5;
	CHECK_MESSAGE(
			vector[Vector3i::Axis::AXIS_Y] == 5,
			"Vector3i array operator setter should work as expected.");
}

TEST_CASE("[Vector3i] Clamp method") {
	const Vector3i vector = Vector3i(10, 10, 10);
	CHECK_MESSAGE(
			Vector3i(-5, 5, 15).clamp(Vector3i(), vector) == Vector3i(0, 5, 10),
			"Vector3i clamp should work as expected.");
	CHECK_MESSAGE(
			vector.clamp(Vector3i(0, 10, 15), Vector3i(5, 10, 20)) == Vector3i(5, 10, 15),
			"Vector3i clamp should work as expected.");
}

TEST_CASE("[Vector3i] Length methods") {
	const Vector3i vector1 = Vector3i(10, 10, 10);
	const Vector3i vector2 = Vector3i(20, 30, 40);
	CHECK_MESSAGE(
			vector1.length_squared() == 300,
			"Vector3i length_squared should work as expected and return exact result.");
	CHECK_MESSAGE(
			vector1.length() == doctest::Approx(10 * Math_SQRT3),
			"Vector3i length should work as expected.");
	CHECK_MESSAGE(
			vector2.length_squared() == 2900,
			"Vector3i length_squared should work as expected and return exact result.");
	CHECK_MESSAGE(
			vector2.length() == doctest::Approx(53.8516480713450403125),
			"Vector3i length should work as expected.");
}

TEST_CASE("[Vector3i] Operators") {
	const Vector3i vector1 = Vector3i(4, 5, 9);
	const Vector3i vector2 = Vector3i(1, 2, 3);

	CHECK_MESSAGE(
			(vector1 + vector2) == Vector3i(5, 7, 12),
			"Vector3i addition with integers should give exact results.");
	CHECK_MESSAGE(
			(vector1 - vector2) == Vector3i(3, 3, 6),
			"Vector3i subtraction with integers should give exact results.");
	CHECK_MESSAGE(
			(vector1 * vector2) == Vector3i(4, 10, 27),
			"Vector3i multiplication with integers should give exact results.");
	CHECK_MESSAGE(
			(vector1 / vector2) == Vector3i(4, 2, 3),
			"Vector3i division with integers should give exact results.");

	CHECK_MESSAGE(
			(vector1 * 2) == Vector3i(8, 10, 18),
			"Vector3i multiplication with integers should give exact results.");
	CHECK_MESSAGE(
			(vector1 / 2) == Vector3i(2, 2, 4),
			"Vector3i division with integers should give exact results.");

	CHECK_MESSAGE(
			((Vector3)vector1) == Vector3(4, 5, 9),
			"Vector3i cast to Vector3 should work as expected.");
	CHECK_MESSAGE(
			((Vector3)vector2) == Vector3(1, 2, 3),
			"Vector3i cast to Vector3 should work as expected.");
	CHECK_MESSAGE(
			Vector3i(Vector3(1.1, 2.9, 3.9)) == Vector3i(1, 2, 3),
			"Vector3i constructed from Vector3 should work as expected.");
}

TEST_CASE("[Vector3i] Other methods") {
	const Vector3i vector = Vector3i(1, 3, -7);

	CHECK_MESSAGE(
			vector.min(Vector3i(3, 2, 5)) == Vector3i(1, 2, -7),
			"Vector3i min should return expected value.");
	CHECK_MESSAGE(
			vector.max(Vector3i(5, 2, 4)) == Vector3i(5, 3, 4),
			"Vector3i max should return expected value.");

	CHECK_MESSAGE(
			vector.snapped(Vector3i(4, 2, 5)) == Vector3i(0, 4, -5),
			"Vector3i snapped should work as expected.");
}

TEST_CASE("[Vector3i] Abs and sign methods") {
	const Vector3i vector1 = Vector3i(1, 3, 5);
	const Vector3i vector2 = Vector3i(1, -3, -5);
	CHECK_MESSAGE(
			vector1.abs() == vector1,
			"Vector3i abs should work as expected.");
	CHECK_MESSAGE(
			vector2.abs() == vector1,
			"Vector3i abs should work as expected.");

	CHECK_MESSAGE(
			vector1.sign() == Vector3i(1, 1, 1),
			"Vector3i sign should work as expected.");
	CHECK_MESSAGE(
			vector2.sign() == Vector3i(1, -1, -1),
			"Vector3i sign should work as expected.");
}
} // namespace TestVector3i

#endif // TEST_VECTOR3I_H
