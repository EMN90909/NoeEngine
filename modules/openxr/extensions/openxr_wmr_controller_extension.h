// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef OPENXR_WMR_CONTROLLER_EXTENSION_H
#define OPENXR_WMR_CONTROLLER_EXTENSION_H

#include "openxr_extension_wrapper.h"

class OpenXRWMRControllerExtension : public OpenXRExtensionWrapper {
public:
	enum WMRControllers {
		WMR_HPMR,
		WMR_SAMSUNG_ODESSY,
		WMR_MAX_CONTROLLERS
	};

	virtual HashMap<String, bool *> get_requested_extensions() override;

	bool is_available(WMRControllers p_type);

	virtual void on_register_metadata() override;

private:
	bool available[WMR_MAX_CONTROLLERS] = { false, false };
};

#endif // OPENXR_WMR_CONTROLLER_EXTENSION_H
