// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef EDITOR_DEBUGGER_SERVER_H
#define EDITOR_DEBUGGER_SERVER_H

#include "core/debugger/remote_debugger_peer.h"
#include "core/object/ref_counted.h"

class EditorDebuggerServer : public RefCounted {
public:
	typedef EditorDebuggerServer *(*CreateServerFunc)(const String &p_uri);

private:
	static HashMap<StringName, CreateServerFunc> protocols;

public:
	static void initialize();
	static void deinitialize();

	static void register_protocol_handler(const String &p_protocol, CreateServerFunc p_func);
	static EditorDebuggerServer *create(const String &p_protocol);

	virtual String get_uri() const = 0;
	virtual void poll() = 0;
	virtual Error start(const String &p_uri) = 0;
	virtual void stop() = 0;
	virtual bool is_active() const = 0;
	virtual bool is_connection_available() const = 0;
	virtual Ref<RemoteDebuggerPeer> take_connection() = 0;
};

#endif // EDITOR_DEBUGGER_SERVER_H
