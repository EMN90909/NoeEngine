// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef EDITOR_FILE_SERVER_H
#define EDITOR_FILE_SERVER_H

#include "core/io/file_access_network.h"
#include "core/io/packet_peer.h"
#include "core/io/tcp_server.h"
#include "core/object/class_db.h"
#include "core/os/thread.h"

class EditorFileServer : public Object {
	GDCLASS(EditorFileServer, Object);

	enum Command {
		CMD_NONE,
		CMD_ACTIVATE,
		CMD_STOP,
	};

	struct ClientData {
		Thread *thread = nullptr;
		Ref<StreamPeerTCP> connection;
		HashMap<int, Ref<FileAccess>> files;
		EditorFileServer *efs = nullptr;
		bool quit = false;
	};

	Ref<TCPServer> server;
	HashSet<Thread *> to_wait;

	static void _close_client(ClientData *cd);
	static void _subthread_start(void *s);

	Mutex wait_mutex;
	Thread thread;
	static void _thread_start(void *);
	bool quit = false;
	Command cmd = CMD_NONE;

	String password;
	int port = 0;
	bool active = false;

public:
	void start();
	void stop();

	bool is_active() const;

	EditorFileServer();
	~EditorFileServer();
};

#endif // EDITOR_FILE_SERVER_H
