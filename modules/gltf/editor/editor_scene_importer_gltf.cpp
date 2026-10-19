// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifdef TOOLS_ENABLED

#include "editor_scene_importer_gltf.h"

#include "../gltf_defines.h"
#include "../gltf_document.h"

uint32_t EditorSceneFormatImporterGLTF::get_import_flags() const {
	return ImportFlags::IMPORT_SCENE | ImportFlags::IMPORT_ANIMATION;
}

void EditorSceneFormatImporterGLTF::get_extensions(List<String> *r_extensions) const {
	r_extensions->push_back("gltf");
	r_extensions->push_back("glb");
}

Node *EditorSceneFormatImporterGLTF::import_scene(const String &p_path, uint32_t p_flags,
		const HashMap<StringName, Variant> &p_options,
		List<String> *r_missing_deps, Error *r_err) {
	Ref<GLTFDocument> gltf;
	gltf.instantiate();
	Ref<GLTFState> state;
	state.instantiate();
	if (p_options.has("gltf/embedded_image_handling")) {
		int32_t enum_option = p_options["gltf/embedded_image_handling"];
		state->set_handle_binary_image(enum_option);
	}
	Error err = gltf->append_from_file(p_path, state, p_flags);
	if (err != OK) {
		if (r_err) {
			*r_err = err;
		}
		return nullptr;
	}
	if (p_options.has("animation/import")) {
		state->set_create_animations(bool(p_options["animation/import"]));
	}

#ifndef DISABLE_DEPRECATED
	bool trimming = p_options.has("animation/trimming") ? (bool)p_options["animation/trimming"] : false;
	bool remove_immutable = p_options.has("animation/remove_immutable_tracks") ? (bool)p_options["animation/remove_immutable_tracks"] : true;
	return gltf->generate_scene(state, (float)p_options["animation/fps"], trimming, remove_immutable);
#else
	return gltf->generate_scene(state, (float)p_options["animation/fps"], (bool)p_options["animation/trimming"], (bool)p_options["animation/remove_immutable_tracks"]);
#endif
}

void EditorSceneFormatImporterGLTF::get_import_options(const String &p_path,
		List<ResourceImporter::ImportOption> *r_options) {
	r_options->push_back(ResourceImporterScene::ImportOption(PropertyInfo(Variant::INT, "gltf/embedded_image_handling", PROPERTY_HINT_ENUM, "Discard All Textures,Extract Textures,Embed As Basis Universal,Embed as Uncompressed", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_UPDATE_ALL_IF_MODIFIED), GLTFState::HANDLE_BINARY_EXTRACT_TEXTURES));
}

#endif // TOOLS_ENABLED
