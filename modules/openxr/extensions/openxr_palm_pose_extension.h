// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef OPENXR_PALM_POSE_EXTENSION_H
#define OPENXR_PALM_POSE_EXTENSION_H

#include "openxr_extension_wrapper.h"

class OpenXRPalmPoseExtension : public OpenXRExtensionWrapper {
public:
	static OpenXRPalmPoseExtension *get_singleton();

	OpenXRPalmPoseExtension();
	virtual ~OpenXRPalmPoseExtension() override;

	virtual HashMap<String, bool *> get_requested_extensions() override;

	bool is_available();

private:
	static OpenXRPalmPoseExtension *singleton;

	bool available = false;
};

#endif // OPENXR_PALM_POSE_EXTENSION_H
