// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "register_types.h"

#include "audio_stream_mp3.h"

#ifdef TOOLS_ENABLED
#include "core/config/engine.h"
#include "resource_importer_mp3.h"
#endif

void initialize_minimp3_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

#ifdef TOOLS_ENABLED
	if (Engine::get_singleton()->is_editor_hint()) {
		Ref<ResourceImporterMP3> mp3_import;
		mp3_import.instantiate();
		ResourceFormatImporter::get_singleton()->add_importer(mp3_import);
	}
#endif
	GDREGISTER_CLASS(AudioStreamMP3);
}

void uninitialize_minimp3_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}
