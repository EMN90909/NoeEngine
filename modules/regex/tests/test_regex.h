// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef TEST_REGEX_H
#define TEST_REGEX_H

#include "core/string/ustring.h"
#include "modules/regex/regex.h"

#include "tests/test_macros.h"

namespace TestRegEx {

TEST_CASE("[RegEx] Initialization") {
	const String pattern = "(?<vowel>[aeiou])";

	RegEx re1(pattern);
	CHECK(re1.is_valid());
	CHECK(re1.get_pattern() == pattern);
	CHECK(re1.get_group_count() == 1);

	PackedStringArray names = re1.get_names();
	CHECK(names.size() == 1);
	CHECK(names[0] == "vowel");

	RegEx re2;
	CHECK(re2.is_valid() == false);
	CHECK(re2.compile(pattern) == OK);
	CHECK(re2.is_valid());

	CHECK(re1.get_pattern() == re2.get_pattern());
	CHECK(re1.get_group_count() == re2.get_group_count());

	names = re2.get_names();
	CHECK(names.size() == 1);
	CHECK(names[0] == "vowel");
}

TEST_CASE("[RegEx] Clearing") {
	RegEx re("Godot");
	REQUIRE(re.is_valid());
	re.clear();
	CHECK(re.is_valid() == false);
}

TEST_CASE("[RegEx] Searching") {
	const String s = "Searching";
	const String vowels = "[aeiou]{1,2}";
	const String numerics = "\\d";

	RegEx re(vowels);
	REQUIRE(re.is_valid());

	Ref<RegExMatch> match = re.search(s);
	REQUIRE(match != nullptr);
	CHECK(match->get_string(0) == "ea");

	match = re.search(s, 2, 4);
	REQUIRE(match != nullptr);
	CHECK(match->get_string(0) == "a");

	const Array all_results = re.search_all(s);
	CHECK(all_results.size() == 2);
	match = all_results[0];
	REQUIRE(match != nullptr);
	CHECK(match->get_string(0) == "ea");
	match = all_results[1];
	REQUIRE(match != nullptr);
	CHECK(match->get_string(0) == "i");

	CHECK(re.compile(numerics) == OK);
	CHECK(re.is_valid());
	CHECK(re.search(s) == nullptr);
	CHECK(re.search_all(s).size() == 0);
}

TEST_CASE("[RegEx] Substitution") {
	String s = "Double all the vowels.";

	RegEx re("(?<vowel>[aeiou])");
	REQUIRE(re.is_valid());
	CHECK(re.sub(s, "$0$vowel", true) == "Doouublee aall thee vooweels.");
}

TEST_CASE("[RegEx] Uninitialized use") {
	const String s = "Godot";

	RegEx re;
	ERR_PRINT_OFF;
	CHECK(re.search(s) == nullptr);
	CHECK(re.search_all(s).size() == 0);
	CHECK(re.sub(s, "") == "");
	CHECK(re.get_group_count() == 0);
	CHECK(re.get_names().size() == 0);
	ERR_PRINT_ON
}

TEST_CASE("[RegEx] Empty Pattern") {
	const String s = "Godot";

	RegEx re;
	CHECK(re.compile("") == OK);
	CHECK(re.is_valid());
}

TEST_CASE("[RegEx] Invalid end position") {
	const String s = "Godot";

	RegEx re("o");
	REQUIRE(re.is_valid());
	Ref<RegExMatch> match = re.search(s, 0, 10);
	CHECK(match->get_string(0) == "o");

	const Array all_results = re.search_all(s, 0, 10);
	CHECK(all_results.size() == 2);
	match = all_results[0];
	REQUIRE(match != nullptr);
	CHECK(match->get_string(0) == String("o"));
	match = all_results[1];
	REQUIRE(match != nullptr);
	CHECK(match->get_string(0) == String("o"));

	CHECK(re.sub(s, "", true, 0, 10) == "Gdt");
}
} // namespace TestRegEx

#endif // TEST_REGEX_H
