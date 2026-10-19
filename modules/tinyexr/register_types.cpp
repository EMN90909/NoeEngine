// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "register_types.h"

#include "image_loader_tinyexr.h"
#include "image_saver_tinyexr.h"

static Ref<ImageLoaderTinyEXR> image_loader_tinyexr;

void initialize_tinyexr_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	image_loader_tinyexr.instantiate();
	ImageLoader::add_image_format_loader(image_loader_tinyexr);

	Image::save_exr_func = save_exr;
	Image::save_exr_buffer_func = save_exr_buffer;
}

void uninitialize_tinyexr_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	ImageLoader::remove_image_format_loader(image_loader_tinyexr);
	image_loader_tinyexr.unref();

	Image::save_exr_func = nullptr;
}
