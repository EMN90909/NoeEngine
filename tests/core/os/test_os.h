// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef TEST_OS_H
#define TEST_OS_H

#include "core/os/os.h"

#include "thirdparty/doctest/doctest.h"

namespace TestOS {

TEST_CASE("[OS] Environment variables") {
#ifdef WINDOWS_ENABLED
	CHECK_MESSAGE(
			OS::get_singleton()->has_environment("USERPROFILE"),
			"The USERPROFILE environment variable should be present.");
#else
	CHECK_MESSAGE(
			OS::get_singleton()->has_environment("HOME"),
			"The HOME environment variable should be present.");
#endif

	OS::get_singleton()->set_environment("HELLO", "world");
	CHECK_MESSAGE(
			OS::get_singleton()->get_environment("HELLO") == "world",
			"The previously-set HELLO environment variable should return the expected value.");
}

TEST_CASE("[OS] Command line arguments") {
	List<String> arguments = OS::get_singleton()->get_cmdline_args();
	bool found = false;
	for (int i = 0; i < arguments.size(); i++) {
		if (arguments[i] == "--test") {
			found = true;
			break;
		}
	}
	CHECK_MESSAGE(
			found,
			"The `--test` option must be present in the list of command line arguments.");
}

TEST_CASE("[OS] Executable and data paths") {
	CHECK_MESSAGE(
			OS::get_singleton()->get_executable_path().is_absolute_path(),
			"The executable path returned should be an absolute path.");
	CHECK_MESSAGE(
			OS::get_singleton()->get_data_path().is_absolute_path(),
			"The user data path returned should be an absolute path.");
	CHECK_MESSAGE(
			OS::get_singleton()->get_config_path().is_absolute_path(),
			"The user configuration path returned should be an absolute path.");
	CHECK_MESSAGE(
			OS::get_singleton()->get_cache_path().is_absolute_path(),
			"The cache path returned should be an absolute path.");
}

TEST_CASE("[OS] Ticks") {
	CHECK_MESSAGE(
			OS::get_singleton()->get_ticks_usec() > 1000,
			"The returned ticks (in microseconds) must be greater than 1,000.");
	CHECK_MESSAGE(
			OS::get_singleton()->get_ticks_msec() > 1,
			"The returned ticks (in milliseconds) must be greater than 1.");
}

TEST_CASE("[OS] Feature tags") {
	CHECK_MESSAGE(
			OS::get_singleton()->has_feature("editor"),
			"The binary has the \"editor\" feature tag.");
	CHECK_MESSAGE(
			!OS::get_singleton()->has_feature("template"),
			"The binary does not have the \"template\" feature tag.");
	CHECK_MESSAGE(
			!OS::get_singleton()->has_feature("template_debug"),
			"The binary does not have the \"template_debug\" feature tag.");
	CHECK_MESSAGE(
			!OS::get_singleton()->has_feature("template_release"),
			"The binary does not have the \"template_release\" feature tag.");
}

TEST_CASE("[OS] Process ID") {
	CHECK_MESSAGE(
			OS::get_singleton()->get_process_id() >= 1,
			"The returned process ID should be greater than zero.");
}

TEST_CASE("[OS] Processor count and memory information") {
	CHECK_MESSAGE(
			OS::get_singleton()->get_processor_count() >= 1,
			"The returned processor count should be greater than zero.");
#ifdef DEBUG_ENABLED
	CHECK_MESSAGE(
			OS::get_singleton()->get_static_memory_usage() >= 1,
			"The returned static memory usage should be greater than zero.");
	CHECK_MESSAGE(
			OS::get_singleton()->get_static_memory_peak_usage() >= 1,
			"The returned static memory peak usage should be greater than zero.");
#endif // DEBUG_ENABLED
}

TEST_CASE("[OS] Execute") {
#ifdef WINDOWS_ENABLED
	List<String> arguments;
	arguments.push_back("/C");
	arguments.push_back("dir > NUL");
	int exit_code;
	const Error err = OS::get_singleton()->execute("cmd", arguments, nullptr, &exit_code);
	CHECK_MESSAGE(
			err == OK,
			"(Running the command `cmd /C \"dir > NUL\"` returns the expected Godot error code (OK).");
	CHECK_MESSAGE(
			exit_code == 0,
			"Running the command `cmd /C \"dir > NUL\"` returns a zero (successful) exit code.");
#else
	List<String> arguments;
	arguments.push_back("-c");
	arguments.push_back("ls > /dev/null");
	int exit_code;
	const Error err = OS::get_singleton()->execute("sh", arguments, nullptr, &exit_code);
	CHECK_MESSAGE(
			err == OK,
			"(Running the command `sh -c \"ls > /dev/null\"` returns the expected Godot error code (OK).");
	CHECK_MESSAGE(
			exit_code == 0,
			"Running the command `sh -c \"ls > /dev/null\"` returns a zero (successful) exit code.");
#endif
}

} // namespace TestOS

#endif // TEST_OS_H
