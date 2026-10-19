// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef TEST_TRANSFORM_3D_H
#define TEST_TRANSFORM_3D_H

#include "core/math/transform_3d.h"

#include "tests/test_macros.h"

namespace TestTransform3D {

Transform3D create_dummy_transform() {
	return Transform3D(Basis(Vector3(1, 2, 3), Vector3(4, 5, 6), Vector3(7, 8, 9)), Vector3(10, 11, 12));
}

Transform3D identity() {
	return Transform3D();
}

TEST_CASE("[Transform3D] translation") {
	Vector3 offset = Vector3(1, 2, 3);

	// Both versions should give the same result applied to identity.
	CHECK(identity().translated(offset) == identity().translated_local(offset));

	// Check both versions against left and right multiplications.
	Transform3D orig = create_dummy_transform();
	Transform3D T = identity().translated(offset);
	CHECK(orig.translated(offset) == T * orig);
	CHECK(orig.translated_local(offset) == orig * T);
}

TEST_CASE("[Transform3D] scaling") {
	Vector3 scaling = Vector3(1, 2, 3);

	// Both versions should give the same result applied to identity.
	CHECK(identity().scaled(scaling) == identity().scaled_local(scaling));

	// Check both versions against left and right multiplications.
	Transform3D orig = create_dummy_transform();
	Transform3D S = identity().scaled(scaling);
	CHECK(orig.scaled(scaling) == S * orig);
	CHECK(orig.scaled_local(scaling) == orig * S);
}

TEST_CASE("[Transform3D] rotation") {
	Vector3 axis = Vector3(1, 2, 3).normalized();
	real_t phi = 1.0;

	// Both versions should give the same result applied to identity.
	CHECK(identity().rotated(axis, phi) == identity().rotated_local(axis, phi));

	// Check both versions against left and right multiplications.
	Transform3D orig = create_dummy_transform();
	Transform3D R = identity().rotated(axis, phi);
	CHECK(orig.rotated(axis, phi) == R * orig);
	CHECK(orig.rotated_local(axis, phi) == orig * R);
}

TEST_CASE("[Transform3D] Finite number checks") {
	const Vector3 y(0, 1, 2);
	const Vector3 infinite_vec(NAN, NAN, NAN);
	const Basis x(y, y, y);
	const Basis infinite_basis(infinite_vec, infinite_vec, infinite_vec);

	CHECK_MESSAGE(
			Transform3D(x, y).is_finite(),
			"Transform3D with all components finite should be finite");

	CHECK_FALSE_MESSAGE(
			Transform3D(x, infinite_vec).is_finite(),
			"Transform3D with one component infinite should not be finite.");
	CHECK_FALSE_MESSAGE(
			Transform3D(infinite_basis, y).is_finite(),
			"Transform3D with one component infinite should not be finite.");

	CHECK_FALSE_MESSAGE(
			Transform3D(infinite_basis, infinite_vec).is_finite(),
			"Transform3D with two components infinite should not be finite.");
}

} // namespace TestTransform3D

#endif // TEST_TRANSFORM_3D_H
