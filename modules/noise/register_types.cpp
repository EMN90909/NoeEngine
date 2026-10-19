// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "register_types.h"

#include "fastnoise_lite.h"
#include "noise.h"
#include "noise_texture_2d.h"

#ifdef TOOLS_ENABLED
#include "editor/editor_plugin.h"
#include "editor/noise_editor_plugin.h"
#endif

void initialize_noise_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
		GDREGISTER_CLASS(NoiseTexture2D);
		GDREGISTER_ABSTRACT_CLASS(Noise);
		GDREGISTER_CLASS(FastNoiseLite);
		ClassDB::add_compatibility_class("NoiseTexture", "NoiseTexture2D");
	}

#ifdef TOOLS_ENABLED
	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR) {
		EditorPlugins::add_by_type<NoiseEditorPlugin>();
	}
#endif
}

void uninitialize_noise_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}
