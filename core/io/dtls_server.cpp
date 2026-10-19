// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "dtls_server.h"

#include "core/config/project_settings.h"
#include "core/io/file_access.h"

DTLSServer *(*DTLSServer::_create)() = nullptr;
bool DTLSServer::available = false;

DTLSServer *DTLSServer::create() {
	if (_create) {
		return _create();
	}
	return nullptr;
}

bool DTLSServer::is_available() {
	return available;
}

void DTLSServer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("setup", "server_options"), &DTLSServer::setup);
	ClassDB::bind_method(D_METHOD("take_connection", "udp_peer"), &DTLSServer::take_connection);
}
