// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "core/string/ustring.h"
#include "main/main.h"
#include "os_ios.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

static OS_IOS *os = nullptr;

int add_path(int p_argc, char **p_args) {
	NSString *str = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"godot_path"];
	if (!str) {
		return p_argc;
	}

	p_args[p_argc++] = (char *)"--path";
	p_args[p_argc++] = (char *)[str cStringUsingEncoding:NSUTF8StringEncoding];
	p_args[p_argc] = nullptr;

	return p_argc;
}

int add_cmdline(int p_argc, char **p_args) {
	NSArray *arr = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"godot_cmdline"];
	if (!arr) {
		return p_argc;
	}

	for (NSUInteger i = 0; i < [arr count]; i++) {
		NSString *str = [arr objectAtIndex:i];
		if (!str) {
			continue;
		}
		p_args[p_argc++] = (char *)[str cStringUsingEncoding:NSUTF8StringEncoding];
	}

	p_args[p_argc] = nullptr;

	return p_argc;
}

int ios_main(int argc, char **argv) {
	size_t len = strlen(argv[0]);

	while (len--) {
		if (argv[0][len] == '/') {
			break;
		}
	}

	if (len >= 0) {
		char path[512];
		memcpy(path, argv[0], len > sizeof(path) ? sizeof(path) : len);
		path[len] = 0;
		printf("Path: %s\n", path);
		chdir(path);
	}

	printf("godot_ios %s\n", argv[0]);
	char cwd[512];
	getcwd(cwd, sizeof(cwd));
	printf("cwd %s\n", cwd);
	os = new OS_IOS();

	// We must override main when testing is enabled
	TEST_MAIN_OVERRIDE

	char *fargv[64];
	for (int i = 0; i < argc; i++) {
		fargv[i] = argv[i];
	}
	fargv[argc] = nullptr;
	argc = add_path(argc, fargv);
	argc = add_cmdline(argc, fargv);

	printf("os created\n");

	Error err = Main::setup(fargv[0], argc - 1, &fargv[1], false);
	printf("setup %i\n", err);

	if (err == ERR_HELP) { // Returned by --help and --version, so success.
		return 0;
	} else if (err != OK) {
		return 255;
	}

	os->initialize_modules();

	return 0;
}

void ios_finish() {
	printf("ios_finish\n");
	Main::cleanup();
	delete os;
}
