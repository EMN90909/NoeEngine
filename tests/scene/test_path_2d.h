// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef TEST_PATH_2D_H
#define TEST_PATH_2D_H

#include "scene/2d/path_2d.h"

#include "tests/test_macros.h"

namespace TestPath2D {

TEST_CASE("[SceneTree][Path2D] Initialization") {
	SUBCASE("Path should be empty right after initialization") {
		Path2D *test_path = memnew(Path2D);
		CHECK(test_path->get_curve() == nullptr);
		memdelete(test_path);
	}
}

TEST_CASE("[SceneTree][Path2D] Curve setter and getter") {
	Path2D *test_path = memnew(Path2D);
	const Ref<Curve2D> &curve = memnew(Curve2D);

	SUBCASE("Curve passed to the class should remain the same") {
		test_path->set_curve(curve);
		CHECK(test_path->get_curve() == curve);
	}
	SUBCASE("Curve passed many times to the class should remain the same") {
		test_path->set_curve(curve);
		test_path->set_curve(curve);
		test_path->set_curve(curve);
		CHECK(test_path->get_curve() == curve);
	}
	SUBCASE("Curve rewrite testing") {
		const Ref<Curve2D> &curve1 = memnew(Curve2D);
		const Ref<Curve2D> &curve2 = memnew(Curve2D);

		test_path->set_curve(curve1);
		test_path->set_curve(curve2);
		CHECK_MESSAGE(test_path->get_curve() != curve1,
				"After rewrite, second curve should be in class");
		CHECK_MESSAGE(test_path->get_curve() == curve2,
				"After rewrite, second curve should be in class");
	}

	SUBCASE("Assign same curve to two paths") {
		Path2D *path2 = memnew(Path2D);

		test_path->set_curve(curve);
		path2->set_curve(curve);
		CHECK_MESSAGE(test_path->get_curve() == path2->get_curve(),
				"Both paths have the same curve.");
		memdelete(path2);
	}

	SUBCASE("Swapping curves between two paths") {
		Path2D *path2 = memnew(Path2D);
		const Ref<Curve2D> &curve1 = memnew(Curve2D);
		const Ref<Curve2D> &curve2 = memnew(Curve2D);

		test_path->set_curve(curve1);
		path2->set_curve(curve2);
		CHECK(test_path->get_curve() == curve1);
		CHECK(path2->get_curve() == curve2);

		// Do the swap
		Ref<Curve2D> temp = test_path->get_curve();
		test_path->set_curve(path2->get_curve());
		path2->set_curve(temp);

		CHECK(test_path->get_curve() == curve2);
		CHECK(path2->get_curve() == curve1);
		memdelete(path2);
	}

	memdelete(test_path);
}

} // namespace TestPath2D

#endif // TEST_PATH_2D_H
