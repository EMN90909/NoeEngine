// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef PACKED_SCENE_TRANSLATION_PARSER_PLUGIN_H
#define PACKED_SCENE_TRANSLATION_PARSER_PLUGIN_H

#include "editor/editor_translation_parser.h"

class PackedSceneEditorTranslationParserPlugin : public EditorTranslationParserPlugin {
	GDCLASS(PackedSceneEditorTranslationParserPlugin, EditorTranslationParserPlugin);

	// Scene Node's properties that contain translation strings.
	HashSet<String> lookup_properties;
	// Properties from specific Nodes that should be ignored.
	HashMap<String, Vector<String>> exception_list;

public:
	virtual Error parse_file(const String &p_path, Vector<String> *r_ids, Vector<Vector<String>> *r_ids_ctx_plural) override;
	virtual void get_recognized_extensions(List<String> *r_extensions) const override;

	PackedSceneEditorTranslationParserPlugin();
};

#endif // PACKED_SCENE_TRANSLATION_PARSER_PLUGIN_H
