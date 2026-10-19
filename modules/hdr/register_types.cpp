// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "register_types.h"

#include "image_loader_hdr.h"

static Ref<ImageLoaderHDR> image_loader_hdr;

void initialize_hdr_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	image_loader_hdr.instantiate();
	ImageLoader::add_image_format_loader(image_loader_hdr);
}

void uninitialize_hdr_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	ImageLoader::remove_image_format_loader(image_loader_hdr);
	image_loader_hdr.unref();
}
