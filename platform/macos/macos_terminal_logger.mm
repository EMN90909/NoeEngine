// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "macos_terminal_logger.h"

#ifdef MACOS_ENABLED

#include <os/log.h>

void MacOSTerminalLogger::log_error(const char *p_function, const char *p_file, int p_line, const char *p_code, const char *p_rationale, bool p_editor_notify, ErrorType p_type) {
	if (!should_log(true)) {
		return;
	}

	const char *err_details;
	if (p_rationale && p_rationale[0]) {
		err_details = p_rationale;
	} else {
		err_details = p_code;
	}

	switch (p_type) {
		case ERR_WARNING:
			os_log_info(OS_LOG_DEFAULT,
					"WARNING: %{public}s\nat: %{public}s (%{public}s:%i)",
					err_details, p_function, p_file, p_line);
			logf_error("\E[1;33mWARNING:\E[0;93m %s\n", err_details);
			logf_error("\E[0;90m     at: %s (%s:%i)\E[0m\n", p_function, p_file, p_line);
			break;
		case ERR_SCRIPT:
			os_log_error(OS_LOG_DEFAULT,
					"SCRIPT ERROR: %{public}s\nat: %{public}s (%{public}s:%i)",
					err_details, p_function, p_file, p_line);
			logf_error("\E[1;35mSCRIPT ERROR:\E[0;95m %s\n", err_details);
			logf_error("\E[0;90m          at: %s (%s:%i)\E[0m\n", p_function, p_file, p_line);
			break;
		case ERR_SHADER:
			os_log_error(OS_LOG_DEFAULT,
					"SHADER ERROR: %{public}s\nat: %{public}s (%{public}s:%i)",
					err_details, p_function, p_file, p_line);
			logf_error("\E[1;36mSHADER ERROR:\E[0;96m %s\n", err_details);
			logf_error("\E[0;90m          at: %s (%s:%i)\E[0m\n", p_function, p_file, p_line);
			break;
		case ERR_ERROR:
		default:
			os_log_error(OS_LOG_DEFAULT,
					"ERROR: %{public}s\nat: %{public}s (%{public}s:%i)",
					err_details, p_function, p_file, p_line);
			logf_error("\E[1;31mERROR:\E[0;91m %s\n", err_details);
			logf_error("\E[0;90m   at: %s (%s:%i)\E[0m\n", p_function, p_file, p_line);
			break;
	}
}

#endif // MACOS_ENABLED
