// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef EDITOR_SCENE_IMPORTER_GLTF_H
#define EDITOR_SCENE_IMPORTER_GLTF_H

#ifdef TOOLS_ENABLED

#include "editor/import/resource_importer_scene.h"

class Animation;
class Node;

class EditorSceneFormatImporterGLTF : public EditorSceneFormatImporter {
	GDCLASS(EditorSceneFormatImporterGLTF, EditorSceneFormatImporter);

public:
	virtual uint32_t get_import_flags() const override;
	virtual void get_extensions(List<String> *r_extensions) const override;
	virtual Node *import_scene(const String &p_path, uint32_t p_flags,
			const HashMap<StringName, Variant> &p_options,
			List<String> *r_missing_deps, Error *r_err = nullptr) override;
	virtual void get_import_options(const String &p_path,
			List<ResourceImporter::ImportOption> *r_options) override;
};

#endif // TOOLS_ENABLED

#endif // EDITOR_SCENE_IMPORTER_GLTF_H
