// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef DEBUG_ADAPTER_SERVER_H
#define DEBUG_ADAPTER_SERVER_H

#include "debug_adapter_protocol.h"
#include "editor/editor_plugin.h"

class DebugAdapterServer : public EditorPlugin {
	GDCLASS(DebugAdapterServer, EditorPlugin);

	DebugAdapterProtocol protocol;

	int remote_port = 6006;
	bool thread_running = false;
	bool started = false;
	bool polling = false;
	static void thread_func(void *p_userdata);

private:
	void _notification(int p_what);

public:
	DebugAdapterServer();
	void start();
	void stop();
};

#endif // DEBUG_ADAPTER_SERVER_H
