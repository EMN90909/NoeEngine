// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef TEST_PROJECT_SETTINGS_H
#define TEST_PROJECT_SETTINGS_H

#include "core/config/project_settings.h"
#include "core/variant/variant.h"
#include "tests/test_macros.h"

namespace TestProjectSettings {

TEST_CASE("[ProjectSettings] Get existing setting") {
	CHECK(ProjectSettings::get_singleton()->has_setting("application/config/name"));

	Variant variant = ProjectSettings::get_singleton()->get_setting("application/config/name");
	CHECK_EQ(variant.get_type(), Variant::STRING);

	String name = variant;
	CHECK_EQ(name, "GDScript Integration Test Suite");
}

TEST_CASE("[ProjectSettings] Default value is ignored if setting exists") {
	CHECK(ProjectSettings::get_singleton()->has_setting("application/config/name"));

	Variant variant = ProjectSettings::get_singleton()->get_setting("application/config/name", "SomeDefaultValue");
	CHECK_EQ(variant.get_type(), Variant::STRING);

	String name = variant;
	CHECK_EQ(name, "GDScript Integration Test Suite");
}

TEST_CASE("[ProjectSettings] Non existing setting is null") {
	CHECK_FALSE(ProjectSettings::get_singleton()->has_setting("not_existing_setting"));

	Variant variant = ProjectSettings::get_singleton()->get_setting("not_existing_setting");
	CHECK_EQ(variant.get_type(), Variant::NIL);
}

TEST_CASE("[ProjectSettings] Non existing setting should return default value") {
	CHECK_FALSE(ProjectSettings::get_singleton()->has_setting("not_existing_setting"));

	Variant variant = ProjectSettings::get_singleton()->get_setting("not_existing_setting");
	CHECK_EQ(variant.get_type(), Variant::NIL);

	variant = ProjectSettings::get_singleton()->get_setting("not_existing_setting", "my_nice_default_value");
	CHECK_EQ(variant.get_type(), Variant::STRING);

	String name = variant;
	CHECK_EQ(name, "my_nice_default_value");

	CHECK_FALSE(ProjectSettings::get_singleton()->has_setting("not_existing_setting"));
}

TEST_CASE("[ProjectSettings] Set value should be returned when retrieved") {
	CHECK_FALSE(ProjectSettings::get_singleton()->has_setting("my_custom_setting"));

	Variant variant = ProjectSettings::get_singleton()->get_setting("my_custom_setting");
	CHECK_EQ(variant.get_type(), Variant::NIL);

	ProjectSettings::get_singleton()->set_setting("my_custom_setting", true);
	CHECK(ProjectSettings::get_singleton()->has_setting("my_custom_setting"));

	variant = ProjectSettings::get_singleton()->get_setting("my_custom_setting");
	CHECK_EQ(variant.get_type(), Variant::BOOL);

	bool value = variant;
	CHECK_EQ(true, value);

	CHECK(ProjectSettings::get_singleton()->has_setting("my_custom_setting"));
}

} // namespace TestProjectSettings

#endif // TEST_PROJECT_SETTINGS_H
