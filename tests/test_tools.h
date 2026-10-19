// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef TEST_TOOLS_H
#define TEST_TOOLS_H

struct ErrorDetector {
	ErrorDetector() {
		eh.errfunc = _detect_error;
		eh.userdata = this;

		add_error_handler(&eh);
	}

	~ErrorDetector() {
		remove_error_handler(&eh);
	}

	void clear() {
		has_error = false;
	}

	static void _detect_error(void *p_self, const char *p_func, const char *p_file, int p_line, const char *p_error, const char *p_errorexp, bool p_editor_notify, ErrorHandlerType p_type) {
		ErrorDetector *self = (ErrorDetector *)p_self;
		self->has_error = true;
	}

	ErrorHandlerList eh;
	bool has_error = false;
};

#endif // TEST_TOOLS_H
