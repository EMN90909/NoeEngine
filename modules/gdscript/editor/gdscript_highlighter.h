// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef GDSCRIPT_HIGHLIGHTER_H
#define GDSCRIPT_HIGHLIGHTER_H

#include "editor/plugins/script_editor_plugin.h"
#include "scene/gui/text_edit.h"

class GDScriptSyntaxHighlighter : public EditorSyntaxHighlighter {
	GDCLASS(GDScriptSyntaxHighlighter, EditorSyntaxHighlighter)

private:
	struct ColorRegion {
		Color color;
		String start_key;
		String end_key;
		bool line_only = false;
	};
	Vector<ColorRegion> color_regions;
	HashMap<int, int> color_region_cache;

	HashMap<StringName, Color> class_names;
	HashMap<StringName, Color> reserved_keywords;
	HashMap<StringName, Color> member_keywords;
	HashSet<StringName> global_functions;

	enum Type {
		NONE,
		REGION,
		NODE_PATH,
		NODE_REF,
		ANNOTATION,
		STRING_NAME,
		SYMBOL,
		NUMBER,
		FUNCTION,
		SIGNAL,
		KEYWORD,
		MEMBER,
		IDENTIFIER,
		TYPE,
	};

	// Colors.
	Color font_color;
	Color symbol_color;
	Color function_color;
	Color global_function_color;
	Color function_definition_color;
	Color built_in_type_color;
	Color number_color;
	Color member_color;
	Color node_path_color;
	Color node_ref_color;
	Color annotation_color;
	Color string_name_color;
	Color type_color;

	void add_color_region(const String &p_start_key, const String &p_end_key, const Color &p_color, bool p_line_only = false);

public:
	virtual void _update_cache() override;
	virtual Dictionary _get_line_syntax_highlighting_impl(int p_line) override;

	virtual String _get_name() const override;
	virtual PackedStringArray _get_supported_languages() const override;

	virtual Ref<EditorSyntaxHighlighter> _create() const override;
};

#endif // GDSCRIPT_HIGHLIGHTER_H
