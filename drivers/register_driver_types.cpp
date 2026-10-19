// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "register_driver_types.h"

#include "core/extension/gdextension_manager.h"
#include "drivers/png/image_loader_png.h"
#include "drivers/png/resource_saver_png.h"

static Ref<ImageLoaderPNG> image_loader_png;
static Ref<ResourceSaverPNG> resource_saver_png;

void register_core_driver_types() {
	image_loader_png.instantiate();
	ImageLoader::add_image_format_loader(image_loader_png);

	resource_saver_png.instantiate();
	ResourceSaver::add_resource_format_saver(resource_saver_png);
}

void unregister_core_driver_types() {
	ImageLoader::remove_image_format_loader(image_loader_png);
	image_loader_png.unref();

	ResourceSaver::remove_resource_format_saver(resource_saver_png);
	resource_saver_png.unref();
}

void register_driver_types() {
}

void unregister_driver_types() {
}
