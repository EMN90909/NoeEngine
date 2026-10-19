// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef GDSCRIPT_TRANSLATION_PARSER_PLUGIN_H
#define GDSCRIPT_TRANSLATION_PARSER_PLUGIN_H

#include "core/templates/hash_set.h"
#include "editor/editor_translation_parser.h"
#include "modules/gdscript/gdscript_parser.h"

class GDScriptEditorTranslationParserPlugin : public EditorTranslationParserPlugin {
	GDCLASS(GDScriptEditorTranslationParserPlugin, EditorTranslationParserPlugin);

	Vector<String> *ids = nullptr;
	Vector<Vector<String>> *ids_ctx_plural = nullptr;

	// List of patterns used for extracting translation strings.
	StringName tr_func = "tr";
	StringName trn_func = "tr_n";
	HashSet<StringName> assignment_patterns;
	HashSet<StringName> first_arg_patterns;
	HashSet<StringName> second_arg_patterns;
	// FileDialog patterns.
	StringName fd_add_filter = "add_filter";
	StringName fd_set_filter = "set_filters";
	StringName fd_filters = "filters";

	void _traverse_class(const GDScriptParser::ClassNode *p_class);
	void _traverse_function(const GDScriptParser::FunctionNode *p_func);
	void _traverse_block(const GDScriptParser::SuiteNode *p_suite);

	void _read_variable(const GDScriptParser::VariableNode *p_var);
	void _assess_expression(GDScriptParser::ExpressionNode *p_expression);
	void _assess_assignment(GDScriptParser::AssignmentNode *p_assignment);
	void _extract_from_call(GDScriptParser::CallNode *p_call);
	void _extract_fd_literals(GDScriptParser::ExpressionNode *p_expression);

public:
	virtual Error parse_file(const String &p_path, Vector<String> *r_ids, Vector<Vector<String>> *r_ids_ctx_plural) override;
	virtual void get_recognized_extensions(List<String> *r_extensions) const override;

	GDScriptEditorTranslationParserPlugin();
};

#endif // GDSCRIPT_TRANSLATION_PARSER_PLUGIN_H
