// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef DEBUGGER_MARSHALLS_H
#define DEBUGGER_MARSHALLS_H

#include "core/object/script_language.h"

struct DebuggerMarshalls {
	struct ScriptStackVariable {
		String name;
		Variant value;
		int type = -1;

		Array serialize(int max_size = 1 << 20); // 1 MiB default.
		bool deserialize(const Array &p_arr);
	};

	struct ScriptStackDump {
		List<ScriptLanguage::StackInfo> frames;
		ScriptStackDump() {}

		Array serialize();
		bool deserialize(const Array &p_arr);
	};

	struct OutputError {
		int hr = -1;
		int min = -1;
		int sec = -1;
		int msec = -1;
		String source_file;
		String source_func;
		int source_line = -1;
		String error;
		String error_descr;
		bool warning = false;
		Vector<ScriptLanguage::StackInfo> callstack;

		Array serialize();
		bool deserialize(const Array &p_arr);
	};
};

#endif // DEBUGGER_MARSHALLS_H
