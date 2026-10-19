// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "register_types.h"

#if defined(WINDOWS_ENABLED)
#include "camera_win.h"
#endif
#if defined(MACOS_ENABLED)
#include "camera_macos.h"
#endif

void initialize_camera_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

#if defined(WINDOWS_ENABLED)
	CameraServer::make_default<CameraWindows>();
#endif
#if defined(MACOS_ENABLED)
	CameraServer::make_default<CameraMacOS>();
#endif
}

void uninitialize_camera_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}
