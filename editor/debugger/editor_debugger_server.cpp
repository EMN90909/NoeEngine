// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "editor_debugger_server.h"

#include "core/io/marshalls.h"
#include "core/io/tcp_server.h"
#include "core/os/mutex.h"
#include "core/os/thread.h"
#include "editor/editor_log.h"
#include "editor/editor_node.h"
#include "editor/editor_settings.h"

class EditorDebuggerServerTCP : public EditorDebuggerServer {
private:
	Ref<TCPServer> server;
	String endpoint;

public:
	static EditorDebuggerServer *create(const String &p_protocol);

	virtual void poll() override {}
	virtual String get_uri() const override;
	virtual Error start(const String &p_uri) override;
	virtual void stop() override;
	virtual bool is_active() const override;
	virtual bool is_connection_available() const override;
	virtual Ref<RemoteDebuggerPeer> take_connection() override;

	EditorDebuggerServerTCP();
};

EditorDebuggerServer *EditorDebuggerServerTCP::create(const String &p_protocol) {
	ERR_FAIL_COND_V(p_protocol != "tcp://", nullptr);
	return memnew(EditorDebuggerServerTCP);
}

EditorDebuggerServerTCP::EditorDebuggerServerTCP() {
	server.instantiate();
}

String EditorDebuggerServerTCP::get_uri() const {
	return endpoint;
}

Error EditorDebuggerServerTCP::start(const String &p_uri) {
	// Default host and port
	String bind_host = (String)EDITOR_GET("network/debug/remote_host");
	int bind_port = (int)EDITOR_GET("network/debug/remote_port");

	// Optionally override
	if (!p_uri.is_empty() && p_uri != "tcp://") {
		String scheme, path;
		Error err = p_uri.parse_url(scheme, bind_host, bind_port, path);
		ERR_FAIL_COND_V(err != OK, ERR_INVALID_PARAMETER);
		ERR_FAIL_COND_V(!bind_host.is_valid_ip_address() && bind_host != "*", ERR_INVALID_PARAMETER);
	}

	// Try listening on ports
	const int max_attempts = 5;
	for (int attempt = 1;; ++attempt) {
		const Error err = server->listen(bind_port, bind_host);
		if (err == OK) {
			break;
		}
		if (attempt >= max_attempts) {
			EditorNode::get_log()->add_message(vformat("Cannot listen on port %d, remote debugging unavailable.", bind_port), EditorLog::MSG_TYPE_ERROR);
			return err;
		}
		int last_port = bind_port++;
		EditorNode::get_log()->add_message(vformat("Cannot listen on port %d, trying %d instead.", last_port, bind_port), EditorLog::MSG_TYPE_WARNING);
	}

	// Endpoint that the client should connect to
	endpoint = vformat("tcp://%s:%d", bind_host, bind_port);

	return OK;
}

void EditorDebuggerServerTCP::stop() {
	server->stop();
}

bool EditorDebuggerServerTCP::is_active() const {
	return server->is_listening();
}

bool EditorDebuggerServerTCP::is_connection_available() const {
	return server->is_listening() && server->is_connection_available();
}

Ref<RemoteDebuggerPeer> EditorDebuggerServerTCP::take_connection() {
	ERR_FAIL_COND_V(!is_connection_available(), Ref<RemoteDebuggerPeer>());
	return memnew(RemoteDebuggerPeerTCP(server->take_connection()));
}

/// EditorDebuggerServer
HashMap<StringName, EditorDebuggerServer::CreateServerFunc> EditorDebuggerServer::protocols;

EditorDebuggerServer *EditorDebuggerServer::create(const String &p_protocol) {
	ERR_FAIL_COND_V(!protocols.has(p_protocol), nullptr);
	return protocols[p_protocol](p_protocol);
}

void EditorDebuggerServer::register_protocol_handler(const String &p_protocol, CreateServerFunc p_func) {
	ERR_FAIL_COND(protocols.has(p_protocol));
	protocols[p_protocol] = p_func;
}

void EditorDebuggerServer::initialize() {
	register_protocol_handler("tcp://", EditorDebuggerServerTCP::create);
}

void EditorDebuggerServer::deinitialize() {
	protocols.clear();
}
