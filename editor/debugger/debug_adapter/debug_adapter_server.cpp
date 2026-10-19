// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "debug_adapter_server.h"

#include "core/os/os.h"
#include "editor/editor_log.h"
#include "editor/editor_node.h"
#include "editor/editor_settings.h"

DebugAdapterServer::DebugAdapterServer() {
	_EDITOR_DEF("network/debug_adapter/remote_port", remote_port);
	_EDITOR_DEF("network/debug_adapter/request_timeout", protocol._request_timeout);
	_EDITOR_DEF("network/debug_adapter/sync_breakpoints", protocol._sync_breakpoints);
}

void DebugAdapterServer::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			start();
		} break;

		case NOTIFICATION_EXIT_TREE: {
			stop();
		} break;

		case NOTIFICATION_INTERNAL_PROCESS: {
			// The main loop can be run again during request processing, which modifies internal state of the protocol.
			// Thus, "polling" is needed to prevent it from parsing other requests while the current one isn't finished.
			if (started && !polling) {
				polling = true;
				protocol.poll();
				polling = false;
			}
		} break;

		case EditorSettings::NOTIFICATION_EDITOR_SETTINGS_CHANGED: {
			protocol._request_timeout = EDITOR_GET("network/debug_adapter/request_timeout");
			protocol._sync_breakpoints = EDITOR_GET("network/debug_adapter/sync_breakpoints");
			int port = (int)_EDITOR_GET("network/debug_adapter/remote_port");
			if (port != remote_port) {
				stop();
				start();
			}
		} break;
	}
}

void DebugAdapterServer::start() {
	remote_port = (int)_EDITOR_GET("network/debug_adapter/remote_port");
	if (protocol.start(remote_port, IPAddress("127.0.0.1")) == OK) {
		EditorNode::get_log()->add_message("--- Debug adapter server started ---", EditorLog::MSG_TYPE_EDITOR);
		set_process_internal(true);
		started = true;
	}
}

void DebugAdapterServer::stop() {
	protocol.stop();
	started = false;
	EditorNode::get_log()->add_message("--- Debug adapter server stopped ---", EditorLog::MSG_TYPE_EDITOR);
}
