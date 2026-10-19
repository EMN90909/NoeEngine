// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "register_types.h"

#include "video_stream_theora.h"

static Ref<ResourceFormatLoaderTheora> resource_loader_theora;

void initialize_theora_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	resource_loader_theora.instantiate();
	ResourceLoader::add_resource_format_loader(resource_loader_theora, true);

	GDREGISTER_CLASS(VideoStreamTheora);
}

void uninitialize_theora_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	ResourceLoader::remove_resource_format_loader(resource_loader_theora);
	resource_loader_theora.unref();
}
