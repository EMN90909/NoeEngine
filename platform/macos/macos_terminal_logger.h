// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef MACOS_TERMINAL_LOGGER_H
#define MACOS_TERMINAL_LOGGER_H

#ifdef MACOS_ENABLED

#include "core/io/logger.h"

class MacOSTerminalLogger : public StdLogger {
public:
	virtual void log_error(const char *p_function, const char *p_file, int p_line, const char *p_code, const char *p_rationale, bool p_editor_notify = false, ErrorType p_type = ERR_ERROR) override;
};

#endif // MACOS_ENABLED

#endif // MACOS_TERMINAL_LOGGER_H
