// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef EDITOR_DEBUGGER_PLUGIN_H
#define EDITOR_DEBUGGER_PLUGIN_H

#include "scene/gui/control.h"

class ScriptEditorDebugger;

class EditorDebuggerSession : public RefCounted {
	GDCLASS(EditorDebuggerSession, RefCounted);

private:
	HashSet<Control *> tabs;

	ScriptEditorDebugger *debugger = nullptr;

	void _breaked(bool p_really_did, bool p_can_debug, String p_message, bool p_has_stackdump);
	void _started();
	void _stopped();
	void _debugger_gone_away();

protected:
	static void _bind_methods();

public:
	void detach_debugger();

	void add_session_tab(Control *p_tab);
	void remove_session_tab(Control *p_tab);
	void send_message(const String &p_message, const Array &p_args = Array());
	void toggle_profiler(const String &p_profiler, bool p_enable, const Array &p_data = Array());
	bool is_breaked();
	bool is_debuggable();
	bool is_active();

	EditorDebuggerSession(ScriptEditorDebugger *p_debugger);
	~EditorDebuggerSession();
};

class EditorDebuggerPlugin : public RefCounted {
	GDCLASS(EditorDebuggerPlugin, RefCounted);

private:
	List<Ref<EditorDebuggerSession>> sessions;

protected:
	static void _bind_methods();

public:
	void create_session(ScriptEditorDebugger *p_debugger);
	void clear();

	virtual void setup_session(int p_idx);
	virtual bool capture(const String &p_message, const Array &p_data, int p_session);
	virtual bool has_capture(const String &p_capture) const;

	Ref<EditorDebuggerSession> get_session(int p_session_id);
	Array get_sessions();

	GDVIRTUAL3R(bool, _capture, const String &, const Array &, int);
	GDVIRTUAL1RC(bool, _has_capture, const String &);
	GDVIRTUAL1(_setup_session, int);

	EditorDebuggerPlugin() {}
	~EditorDebuggerPlugin();
};

#endif // EDITOR_DEBUGGER_PLUGIN_H
