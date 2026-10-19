// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "register_types.h"

#include "image_loader_bmp.h"

static Ref<ImageLoaderBMP> image_loader_bmp;

void initialize_bmp_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	image_loader_bmp.instantiate();
	ImageLoader::add_image_format_loader(image_loader_bmp);
}

void uninitialize_bmp_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	ImageLoader::remove_image_format_loader(image_loader_bmp);
	image_loader_bmp.unref();
}
