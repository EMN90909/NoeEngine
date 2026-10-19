// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef GDSCRIPT_LANGUAGE_SERVER_H
#define GDSCRIPT_LANGUAGE_SERVER_H

#include "../gdscript_parser.h"
#include "editor/editor_plugin.h"
#include "gdscript_language_protocol.h"

class GDScriptLanguageServer : public EditorPlugin {
	GDCLASS(GDScriptLanguageServer, EditorPlugin);

	GDScriptLanguageProtocol protocol;

	Thread thread;
	bool thread_running = false;
	bool started = false;
	bool use_thread = false;
	String host = "127.0.0.1";
	int port = 6005;
	static void thread_main(void *p_userdata);

private:
	void _notification(int p_what);

public:
	GDScriptLanguageServer();
	void start();
	void stop();
};

void register_lsp_types();

#endif // GDSCRIPT_LANGUAGE_SERVER_H
