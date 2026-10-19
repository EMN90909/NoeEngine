// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef EDITOR_SCENE_IMPORTER_FBX_H
#define EDITOR_SCENE_IMPORTER_FBX_H

#ifdef TOOLS_ENABLED

#include "editor/editor_file_system.h"
#include "editor/fbx_importer_manager.h"
#include "editor/import/resource_importer_scene.h"

class Animation;
class Node;

class EditorSceneFormatImporterFBX : public EditorSceneFormatImporter {
	GDCLASS(EditorSceneFormatImporterFBX, EditorSceneFormatImporter);

public:
	virtual uint32_t get_import_flags() const override;
	virtual void get_extensions(List<String> *r_extensions) const override;
	virtual Node *import_scene(const String &p_path, uint32_t p_flags,
			const HashMap<StringName, Variant> &p_options,
			List<String> *r_missing_deps, Error *r_err = nullptr) override;
	virtual void get_import_options(const String &p_path,
			List<ResourceImporter::ImportOption> *r_options) override;
	virtual Variant get_option_visibility(const String &p_path, bool p_for_animation, const String &p_option,
			const HashMap<StringName, Variant> &p_options) override;
};

class EditorFileSystemImportFormatSupportQueryFBX : public EditorFileSystemImportFormatSupportQuery {
	GDCLASS(EditorFileSystemImportFormatSupportQueryFBX, EditorFileSystemImportFormatSupportQuery);

public:
	virtual bool is_active() const override;
	virtual Vector<String> get_file_extensions() const override;
	virtual bool query() override;
};

#endif // TOOLS_ENABLED

#endif // EDITOR_SCENE_IMPORTER_FBX_H
