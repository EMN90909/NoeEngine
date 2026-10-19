// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "main/main.h"

#include "os_macos.h"

#include <string.h>
#include <unistd.h>

#if defined(SANITIZERS_ENABLED)
#include <sys/resource.h>
#endif

int main(int argc, char **argv) {
#if defined(VULKAN_ENABLED)
	// MoltenVK - enable full component swizzling support.
	setenv("MVK_CONFIG_FULL_IMAGE_VIEW_SWIZZLE", "1", 1);
#endif

#if defined(SANITIZERS_ENABLED)
	// Note: Set stack size to be at least 30 MB (vs 8 MB default) to avoid overflow, address sanitizer can increase stack usage up to 3 times.
	struct rlimit stack_lim = { 0x1E00000, 0x1E00000 };
	setrlimit(RLIMIT_STACK, &stack_lim);
#endif

	int first_arg = 1;
	const char *dbg_arg = "-NSDocumentRevisionsDebugMode";
	for (int i = 0; i < argc; i++) {
		if (strcmp(dbg_arg, argv[i]) == 0) {
			first_arg = i + 2;
		}
	}

	OS_MacOS os;
	Error err;

	// We must override main when testing is enabled.
	TEST_MAIN_OVERRIDE

	err = Main::setup(argv[0], argc - first_arg, &argv[first_arg]);

	if (err == ERR_HELP) { // Returned by --help and --version, so success.
		return 0;
	} else if (err != OK) {
		return 255;
	}

	if (Main::start()) {
		os.run(); // It is actually the OS that decides how to run.
	}

	Main::cleanup();

	return os.get_exit_code();
}
