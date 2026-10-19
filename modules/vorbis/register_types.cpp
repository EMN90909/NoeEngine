// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "register_types.h"

#include "audio_stream_ogg_vorbis.h"
#include "resource_importer_ogg_vorbis.h"

void initialize_vorbis_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

#ifdef TOOLS_ENABLED
	if (Engine::get_singleton()->is_editor_hint()) {
		Ref<ResourceImporterOggVorbis> ogg_vorbis_importer;
		ogg_vorbis_importer.instantiate();
		ResourceFormatImporter::get_singleton()->add_importer(ogg_vorbis_importer);
	}
#endif
	GDREGISTER_CLASS(AudioStreamOggVorbis);
	GDREGISTER_CLASS(AudioStreamPlaybackOggVorbis);
}

void uninitialize_vorbis_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}
