// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef DEBUGGER_EDITOR_PLUGIN_H
#define DEBUGGER_EDITOR_PLUGIN_H

#include "editor/editor_plugin.h"

class EditorFileServer;
class MenuButton;
class PopupMenu;

class DebuggerEditorPlugin : public EditorPlugin {
	GDCLASS(DebuggerEditorPlugin, EditorPlugin);

private:
	PopupMenu *debug_menu = nullptr;
	EditorFileServer *file_server = nullptr;
	PopupMenu *instances_menu = nullptr;

	enum MenuOptions {
		RUN_FILE_SERVER,
		RUN_LIVE_DEBUG,
		RUN_DEBUG_COLLISIONS,
		RUN_DEBUG_PATHS,
		RUN_DEBUG_NAVIGATION,
		RUN_DEPLOY_REMOTE_DEBUG,
		RUN_RELOAD_SCRIPTS,
		SERVER_KEEP_OPEN,
	};

	void _update_debug_options();
	void _notification(int p_what);
	void _select_run_count(int p_index);
	void _menu_option(int p_option);

public:
	virtual String get_name() const override { return "Debugger"; }
	bool has_main_screen() const override { return false; }

	DebuggerEditorPlugin(PopupMenu *p_menu);
	~DebuggerEditorPlugin();
};

#endif // DEBUGGER_EDITOR_PLUGIN_H
