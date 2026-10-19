// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef LOCAL_DEBUGGER_H
#define LOCAL_DEBUGGER_H

#include "core/debugger/engine_debugger.h"
#include "core/object/script_language.h"
#include "core/templates/list.h"

class LocalDebugger : public EngineDebugger {
private:
	struct ScriptsProfiler;

	ScriptsProfiler *scripts_profiler = nullptr;

	String target_function;
	HashMap<String, String> options;

	Pair<String, int> to_breakpoint(const String &p_line);
	void print_variables(const List<String> &names, const List<Variant> &values, const String &variable_prefix);

public:
	void debug(bool p_can_continue, bool p_is_error_breakpoint);
	void send_message(const String &p_message, const Array &p_args);
	void send_error(const String &p_func, const String &p_file, int p_line, const String &p_err, const String &p_descr, bool p_editor_notify, ErrorHandlerType p_type);

	LocalDebugger();
	~LocalDebugger();
};

#endif // LOCAL_DEBUGGER_H
