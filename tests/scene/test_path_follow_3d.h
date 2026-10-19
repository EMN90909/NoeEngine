// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef TEST_PATH_FOLLOW_3D_H
#define TEST_PATH_FOLLOW_3D_H

#include "scene/3d/path_3d.h"

#include "tests/test_macros.h"

namespace TestPathFollow3D {

TEST_CASE("[PathFollow3D] Sampling with progress ratio") {
	const Ref<Curve3D> &curve = memnew(Curve3D());
	curve->add_point(Vector3(0, 0, 0));
	curve->add_point(Vector3(100, 0, 0));
	curve->add_point(Vector3(100, 100, 0));
	curve->add_point(Vector3(100, 100, 100));
	curve->add_point(Vector3(100, 0, 100));
	const Path3D *path = memnew(Path3D);
	path->set_curve(curve);
	const PathFollow3D *path_follow_3d = memnew(PathFollow3D);
	path->add_child(path_follow_3d);

	path_follow_3d->set_progress_ratio(0);
	CHECK(path_follow_3d->get_transform().get_origin().is_equal_approx(Vector3(0, 0, 0));

	path_follow_3d->set_progress_ratio(0.125);
	CHECK(path_follow_3d->get_transform().get_origin().is_equal_approx(Vector3(50, 0, 0));

	path_follow_3d->set_progress_ratio(0.25);
	CHECK(path_follow_3d->get_transform().get_origin().is_equal_approx(Vector3(100, 0, 0);

	path_follow_3d->set_progress_ratio(0.375);
	CHECK(path_follow_3d->get_transform().get_origin().is_equal_approx(Vector3(100, 50, 0)));

	path_follow_3d->set_progress_ratio(0.5);
	CHECK(path_follow_3d->get_transform().get_origin().is_equal_approx(Vector3(100, 100, 0)));

	path_follow_3d->set_progress_ratio(0.625);
	CHECK(path_follow_3d->get_transform().get_origin().is_equal_approx(Vector3(100, 100, 50)));

	path_follow_3d->set_progress_ratio(0.75);
	CHECK(path_follow_3d->get_transform().get_origin().is_equal_approx(Vector3(100, 100, 100)));

	path_follow_3d->set_progress_ratio(0.875);
	CHECK(path_follow_3d->get_transform().get_origin().is_equal_approx(Vector3(100, 50, 100)));

	path_follow_3d->set_progress_ratio(1);
	CHECK(path_follow_3d->get_transform().get_origin().is_equal_approx(Vector3(100, 0, 100)));

	memdelete(path);
}

TEST_CASE("[PathFollow3D] Sampling with progress") {
	const Ref<Curve3D> &curve = memnew(Curve3D());
	curve->add_point(Vector3(0, 0, 0));
	curve->add_point(Vector3(100, 0, 0));
	curve->add_point(Vector3(100, 100, 0));
	curve->add_point(Vector3(100, 100, 100));
	curve->add_point(Vector3(100, 0, 100));
	const Path3D *path = memnew(Path3D);
	path->set_curve(curve);
	const PathFollow3D *path_follow_3d = memnew(PathFollow3D);
	path->add_child(path_follow_3d);

	path_follow_3d->set_progress(0);
	CHECK(path_follow_3d->get_transform().get_origin().is_equal_approx(Vector3(0, 0, 0));

	path_follow_3d->set_progress(50);
	CHECK(path_follow_3d->get_transform().get_origin().is_equal_approx(Vector3(50, 0, 0));

	path_follow_3d->set_progress(100);
	CHECK(path_follow_3d->get_transform().get_origin().is_equal_approx(Vector3(100, 0, 0);

	path_follow_3d->set_progress(150);
	CHECK(path_follow_3d->get_transform().get_origin().is_equal_approx(Vector3(100, 50, 0)));

	path_follow_3d->set_progress(200);
	CHECK(path_follow_3d->get_transform().get_origin().is_equal_approx(Vector3(100, 100, 0)));

	path_follow_3d->set_progress(250);
	CHECK(path_follow_3d->get_transform().get_origin().is_equal_approx(Vector3(100, 100, 50)));

	path_follow_3d->set_progress(300);
	CHECK(path_follow_3d->get_transform().get_origin().is_equal_approx(Vector3(100, 100, 100)));

	path_follow_3d->set_progress(350);
	CHECK(path_follow_3d->get_transform().get_origin().is_equal_approx(Vector3(100, 50, 100)));

	path_follow_3d->set_progress(400);
	CHECK(path_follow_3d->get_transform().get_origin().is_equal_approx(Vector3(100, 0, 100)));

	memdelete(path);
}

TEST_CASE("[PathFollow3D] Removal of a point in curve") {
	const Ref<Curve3D> &curve = memnew(Curve3D());
	curve->add_point(Vector3(0, 0, 0));
	curve->add_point(Vector3(100, 0, 0));
	curve->add_point(Vector3(100, 100, 0));
	const Path3D *path = memnew(Path3D);
	path->set_curve(curve);
	const PathFollow3D *path_follow_3d = memnew(PathFollow3D);
	path->add_child(path_follow_3d);

	path_follow_3d->set_progress_ratio(0.5);
	CHECK(path_follow_3d->get_transform().get_origin().is_equal_approx(Vector2(100, 0, 0)));

	curve->remove_point(1);

	CHECK_MESSAGE(
			path_follow_3d->get_transform().get_origin().is_equal_approx(Vector2(50, 50, 0)),
			"Path follow's position should be updated after removing a point from the curve");

	memdelete(path);
}

TEST_CASE("[PathFollow3D] Progress ratio out of range") {
	const Ref<Curve3D> &curve = memnew(Curve3D());
	curve->add_point(Vector3(0, 0, 0));
	curve->add_point(Vector3(100, 0, 0));
	const Path3D *path = memnew(Path3D);
	path->set_curve(curve);
	const PathFollow3D *path_follow_3d = memnew(PathFollow3D);
	path->add_child(path_follow_3d);

	path_follow_3d->set_loop(true);

	path_follow_3d->set_progress_ratio(-0.3);
	CHECK_MESSAGE(
			path_follow_3d->get_progress_ratio() == 0.7,
			"Progress Ratio should loop back from the end in the opposite direction");

	path_follow_3d->set_progress_ratio(1.3);
	CHECK_MESSAGE(
			path_follow_3d->get_progress_ratio() == 0.3,
			"Progress Ratio should loop back from the end in the opposite direction");

	path_follow_3d->set_loop(false);

	path_follow_3d->set_progress_ratio(-0.3);
	CHECK_MESSAGE(
			path_follow_3d->get_progress_ratio() == 0,
			"Progress Ratio should be clamped at 0");

	path_follow_3d->set_progress_ratio(1.3);
	CHECK_MESSAGE(
			path_follow_3d->get_progress_ratio() == 1,
			"Progress Ratio should be clamped at 1");

	memdelete(path);
}

TEST_CASE("[PathFollow3D] Progress out of range") {
	const Ref<Curve3D> &curve = memnew(Curve3D());
	curve->add_point(Vector3(0, 0, 0));
	curve->add_point(Vector3(100, 0, 0));
	const Path3D *path = memnew(Path3D);
	path->set_curve(curve);
	const PathFollow3D *path_follow_3d = memnew(PathFollow3D);
	path->add_child(path_follow_3d);

	path_follow_3d->set_loop(true);

	path_follow_3d->set_progress(-50);
	CHECK_MESSAGE(
			path_follow_3d->get_progress() == 50,
			"Progress should loop back from the end in the opposite direction");

	path_follow_3d->set_progress(150);
	CHECK_MESSAGE(
			path_follow_3d->get_progress() == 50,
			"Progress should loop back from the end in the opposite direction");

	path_follow_3d->set_loop(false);

	path_follow_3d->set_progress(-50);
	CHECK_MESSAGE(
			path_follow_3d->get_progress() == 0,
			"Progress should be clamped at 0");

	path_follow_3d->set_progress(150);
	CHECK_MESSAGE(
			path_follow_3d->get_progress() == 100,
			"Progress should be clamped at max value of curve");

	memdelete(path);
}
} // namespace TestPathFollow3D

#endif // TEST_PATH_FOLLOW_3D_H
