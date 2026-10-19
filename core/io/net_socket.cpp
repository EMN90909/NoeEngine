// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "net_socket.h"

NetSocket *(*NetSocket::_create)() = nullptr;

NetSocket *NetSocket::create() {
	if (_create) {
		return _create();
	}

	ERR_PRINT("Unable to create network socket, platform not supported");
	return nullptr;
}
