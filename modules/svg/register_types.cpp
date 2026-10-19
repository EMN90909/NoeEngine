// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "register_types.h"

#include "image_loader_svg.h"

#include <thorvg.h>

static Ref<ImageLoaderSVG> image_loader_svg;

void initialize_svg_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	tvg::CanvasEngine tvgEngine = tvg::CanvasEngine::Sw;
	if (tvg::Initializer::init(tvgEngine, 1) != tvg::Result::Success) {
		return;
	}

	image_loader_svg.instantiate();
	ImageLoader::add_image_format_loader(image_loader_svg);
}

void uninitialize_svg_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	if (image_loader_svg.is_null()) {
		// It failed to initialize so it was not added.
		return;
	}

	ImageLoader::remove_image_format_loader(image_loader_svg);
	image_loader_svg.unref();
	tvg::Initializer::term(tvg::CanvasEngine::Sw);
}
