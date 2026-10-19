// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef EDITOR_SCENE_IMPORTER_BLEND_H
#define EDITOR_SCENE_IMPORTER_BLEND_H

#ifdef TOOLS_ENABLED

#include "editor/editor_file_system.h"
#include "editor/import/resource_importer_scene.h"

class Animation;
class Node;
class ConfirmationDialog;

class EditorSceneFormatImporterBlend : public EditorSceneFormatImporter {
	GDCLASS(EditorSceneFormatImporterBlend, EditorSceneFormatImporter);

public:
	enum {
		BLEND_VISIBLE_ALL,
		BLEND_VISIBLE_VISIBLE_ONLY,
		BLEND_VISIBLE_RENDERABLE
	};
	enum {
		BLEND_BONE_INFLUENCES_NONE,
		BLEND_BONE_INFLUENCES_COMPATIBLE,
		BLEND_BONE_INFLUENCES_ALL
	};
	enum {
		BLEND_MATERIAL_EXPORT_PLACEHOLDER,
		BLEND_MATERIAL_EXPORT_EXPORT
	};
	enum {
		BLEND_MODIFIERS_NONE,
		BLEND_MODIFIERS_ALL
	};

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

class LineEdit;
class Button;
class EditorFileDialog;
class Label;

class EditorFileSystemImportFormatSupportQueryBlend : public EditorFileSystemImportFormatSupportQuery {
	GDCLASS(EditorFileSystemImportFormatSupportQueryBlend, EditorFileSystemImportFormatSupportQuery);

	ConfirmationDialog *configure_blender_dialog = nullptr;
	LineEdit *blender_path = nullptr;
	Button *blender_path_browse = nullptr;
	EditorFileDialog *browse_dialog = nullptr;
	Label *path_status = nullptr;
	bool confirmed = false;

	String auto_detected_path;
	void _validate_path(String p_path);

	bool _autodetect_path(String p_path);

	void _path_confirmed();

	void _select_install(String p_path);
	void _browse_install();

public:
	virtual bool is_active() const override;
	virtual Vector<String> get_file_extensions() const override;
	virtual bool query() override;

	EditorFileSystemImportFormatSupportQueryBlend();
};

#endif // TOOLS_ENABLED

#endif // EDITOR_SCENE_IMPORTER_BLEND_H
