// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef OPENXR_ANDROID_EXTENSION_H
#define OPENXR_ANDROID_EXTENSION_H

#include "../util.h"
#include "openxr_extension_wrapper.h"

class OpenXRAndroidExtension : public OpenXRExtensionWrapper {
public:
	static OpenXRAndroidExtension *get_singleton();

	OpenXRAndroidExtension();

	virtual HashMap<String, bool *> get_requested_extensions() override;
	virtual void on_before_instance_created() override;
	virtual void *set_instance_create_info_and_get_next_pointer(void *p_next_pointer) override;

	virtual ~OpenXRAndroidExtension() override;

private:
	static OpenXRAndroidExtension *singleton;

	bool loader_init_extension_available = false;
	bool create_instance_extension_available = false;

	// Initialize the loader
	EXT_PROTO_XRRESULT_FUNC1(xrInitializeLoaderKHR, (const XrLoaderInitInfoBaseHeaderKHR *), loaderInitInfo)
};

#endif // OPENXR_ANDROID_EXTENSION_H
