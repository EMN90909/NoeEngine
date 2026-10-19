// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef CAMERA_WIN_H
#define CAMERA_WIN_H

#include "servers/camera/camera_feed.h"
#include "servers/camera_server.h"

class CameraWindows : public CameraServer {
private:
	void add_active_cameras();

public:
	CameraWindows();
	~CameraWindows() {}
};

#endif // CAMERA_WIN_H
