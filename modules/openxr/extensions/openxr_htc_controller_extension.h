// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef OPENXR_HTC_CONTROLLER_EXTENSION_H
#define OPENXR_HTC_CONTROLLER_EXTENSION_H

#include "openxr_extension_wrapper.h"

class OpenXRHTCControllerExtension : public OpenXRExtensionWrapper {
public:
	enum HTCControllers {
		// Note, HTC Vive Wand controllers are part of the core spec and not part of our extension.
		HTC_VIVE_COSMOS,
		HTC_VIVE_FOCUS3,
		HTC_MAX_CONTROLLERS
	};

	virtual HashMap<String, bool *> get_requested_extensions() override;

	bool is_available(HTCControllers p_type);

	virtual void on_register_metadata() override;

private:
	bool available[HTC_MAX_CONTROLLERS] = { false, false };
};

#endif // OPENXR_HTC_CONTROLLER_EXTENSION_H
