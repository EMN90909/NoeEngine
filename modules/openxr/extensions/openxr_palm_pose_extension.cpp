// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "openxr_palm_pose_extension.h"
#include "core/string/print_string.h"

OpenXRPalmPoseExtension *OpenXRPalmPoseExtension::singleton = nullptr;

OpenXRPalmPoseExtension *OpenXRPalmPoseExtension::get_singleton() {
	return singleton;
}

OpenXRPalmPoseExtension::OpenXRPalmPoseExtension() {
	singleton = this;
}

OpenXRPalmPoseExtension::~OpenXRPalmPoseExtension() {
	singleton = nullptr;
}

HashMap<String, bool *> OpenXRPalmPoseExtension::get_requested_extensions() {
	HashMap<String, bool *> request_extensions;

	request_extensions[XR_EXT_PALM_POSE_EXTENSION_NAME] = &available;

	return request_extensions;
}

bool OpenXRPalmPoseExtension::is_available() {
	return available;
}
