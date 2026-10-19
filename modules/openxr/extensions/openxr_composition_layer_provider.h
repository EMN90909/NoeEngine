// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef OPENXR_COMPOSITION_LAYER_PROVIDER_H
#define OPENXR_COMPOSITION_LAYER_PROVIDER_H

#include "openxr_extension_wrapper.h"
#include <openxr/openxr.h>

// Interface for OpenXR extensions that provide a composition layer.
class OpenXRCompositionLayerProvider {
public:
	virtual XrCompositionLayerBaseHeader *get_composition_layer() = 0;

	virtual ~OpenXRCompositionLayerProvider() {}
};

#endif // OPENXR_COMPOSITION_LAYER_PROVIDER_H
