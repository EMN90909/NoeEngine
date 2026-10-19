// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "register_types.h"

#include "texture_loader_dds.h"

static Ref<ResourceFormatDDS> resource_loader_dds;

void initialize_dds_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	resource_loader_dds.instantiate();
	ResourceLoader::add_resource_format_loader(resource_loader_dds);
}

void uninitialize_dds_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	ResourceLoader::remove_resource_format_loader(resource_loader_dds);
	resource_loader_dds.unref();
}
