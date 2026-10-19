// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "openxr_composition_layer_depth_extension.h"

OpenXRCompositionLayerDepthExtension *OpenXRCompositionLayerDepthExtension::singleton = nullptr;

OpenXRCompositionLayerDepthExtension *OpenXRCompositionLayerDepthExtension::get_singleton() {
	return singleton;
}

OpenXRCompositionLayerDepthExtension::OpenXRCompositionLayerDepthExtension() {
	singleton = this;
}

OpenXRCompositionLayerDepthExtension::~OpenXRCompositionLayerDepthExtension() {
	singleton = nullptr;
}

HashMap<String, bool *> OpenXRCompositionLayerDepthExtension::get_requested_extensions() {
	HashMap<String, bool *> request_extensions;

	request_extensions[XR_KHR_COMPOSITION_LAYER_DEPTH_EXTENSION_NAME] = &available;

	return request_extensions;
}

bool OpenXRCompositionLayerDepthExtension::is_available() {
	return available;
}

XrCompositionLayerBaseHeader *OpenXRCompositionLayerDepthExtension::get_composition_layer() {
	// Seems this is all done in our base layer... Just in case this changes...

	return nullptr;
}
