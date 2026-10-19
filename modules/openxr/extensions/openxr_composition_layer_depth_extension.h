// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef OPENXR_COMPOSITION_LAYER_DEPTH_EXTENSION_H
#define OPENXR_COMPOSITION_LAYER_DEPTH_EXTENSION_H

#include "openxr_composition_layer_provider.h"
#include "openxr_extension_wrapper.h"

class OpenXRCompositionLayerDepthExtension : public OpenXRExtensionWrapper, public OpenXRCompositionLayerProvider {
public:
	static OpenXRCompositionLayerDepthExtension *get_singleton();

	OpenXRCompositionLayerDepthExtension();
	virtual ~OpenXRCompositionLayerDepthExtension() override;

	virtual HashMap<String, bool *> get_requested_extensions() override;
	bool is_available();
	virtual XrCompositionLayerBaseHeader *get_composition_layer() override;

private:
	static OpenXRCompositionLayerDepthExtension *singleton;

	bool available = false;
};

#endif // OPENXR_COMPOSITION_LAYER_DEPTH_EXTENSION_H
