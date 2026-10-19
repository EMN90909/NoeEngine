// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "script_debugger.h"

#include "core/debugger/engine_debugger.h"

void ScriptDebugger::set_lines_left(int p_left) {
	lines_left = p_left;
}

int ScriptDebugger::get_lines_left() const {
	return lines_left;
}

void ScriptDebugger::set_depth(int p_depth) {
	depth = p_depth;
}

int ScriptDebugger::get_depth() const {
	return depth;
}

void ScriptDebugger::insert_breakpoint(int p_line, const StringName &p_source) {
	if (!breakpoints.has(p_line)) {
		breakpoints[p_line] = HashSet<StringName>();
	}
	breakpoints[p_line].insert(p_source);
}

void ScriptDebugger::remove_breakpoint(int p_line, const StringName &p_source) {
	if (!breakpoints.has(p_line)) {
		return;
	}

	breakpoints[p_line].erase(p_source);
	if (breakpoints[p_line].size() == 0) {
		breakpoints.erase(p_line);
	}
}

bool ScriptDebugger::is_breakpoint(int p_line, const StringName &p_source) const {
	if (!breakpoints.has(p_line)) {
		return false;
	}
	return breakpoints[p_line].has(p_source);
}

String ScriptDebugger::breakpoint_find_source(const String &p_source) const {
	return p_source;
}

void ScriptDebugger::clear_breakpoints() {
	breakpoints.clear();
}

void ScriptDebugger::set_skip_breakpoints(bool p_skip_breakpoints) {
	skip_breakpoints = p_skip_breakpoints;
}

bool ScriptDebugger::is_skipping_breakpoints() {
	return skip_breakpoints;
}

void ScriptDebugger::debug(ScriptLanguage *p_lang, bool p_can_continue, bool p_is_error_breakpoint) {
	ScriptLanguage *prev = break_lang;
	break_lang = p_lang;
	EngineDebugger::get_singleton()->debug(p_can_continue, p_is_error_breakpoint);
	break_lang = prev;
}

void ScriptDebugger::send_error(const String &p_func, const String &p_file, int p_line, const String &p_err, const String &p_descr, bool p_editor_notify, ErrorHandlerType p_type, const Vector<StackInfo> &p_stack_info) {
	// Store stack info, this is ugly, but allows us to separate EngineDebugger and ScriptDebugger. There might be a better way.
	error_stack_info.append_array(p_stack_info);
	EngineDebugger::get_singleton()->send_error(p_func, p_file, p_line, p_err, p_descr, p_editor_notify, p_type);
	error_stack_info.clear();
}

Vector<ScriptLanguage::StackInfo> ScriptDebugger::get_error_stack_info() const {
	return error_stack_info;
}

ScriptLanguage *ScriptDebugger::get_break_language() const {
	return break_lang;
}
