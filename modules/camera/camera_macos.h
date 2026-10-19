// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef CAMERA_MACOS_H
#define CAMERA_MACOS_H

///@TODO this is a near duplicate of CameraIOS, we should find a way to combine those to minimize code duplication!!!!
// If you fix something here, make sure you fix it there as well!

#include "servers/camera_server.h"

class CameraMacOS : public CameraServer {
public:
	CameraMacOS();

	void update_feeds();
};

#endif // CAMERA_MACOS_H
