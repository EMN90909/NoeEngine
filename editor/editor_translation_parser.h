// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef EDITOR_TRANSLATION_PARSER_H
#define EDITOR_TRANSLATION_PARSER_H

#include "core/error/error_list.h"
#include "core/object/gdvirtual.gen.inc"
#include "core/object/ref_counted.h"
#include "core/object/script_language.h"
#include "core/variant/typed_array.h"

class EditorTranslationParserPlugin : public RefCounted {
	GDCLASS(EditorTranslationParserPlugin, RefCounted);

protected:
	static void _bind_methods();

	GDVIRTUAL3(_parse_file, String, TypedArray<String>, TypedArray<Array>)
	GDVIRTUAL0RC(Vector<String>, _get_recognized_extensions)

public:
	virtual Error parse_file(const String &p_path, Vector<String> *r_ids, Vector<Vector<String>> *r_ids_ctx_plural);
	virtual void get_recognized_extensions(List<String> *r_extensions) const;
};

class EditorTranslationParser {
	static EditorTranslationParser *singleton;

public:
	enum ParserType {
		STANDARD, // GDScript, CSharp, ...
		CUSTOM // User-defined parser plugins. This will override standard parsers if the same extension type is defined.
	};

	static EditorTranslationParser *get_singleton();

	Vector<Ref<EditorTranslationParserPlugin>> standard_parsers;
	Vector<Ref<EditorTranslationParserPlugin>> custom_parsers;

	void get_recognized_extensions(List<String> *r_extensions) const;
	bool can_parse(const String &p_extension) const;
	Ref<EditorTranslationParserPlugin> get_parser(const String &p_extension) const;
	void add_parser(const Ref<EditorTranslationParserPlugin> &p_parser, ParserType p_type);
	void remove_parser(const Ref<EditorTranslationParserPlugin> &p_parser, ParserType p_type);
	void clean_parsers();

	EditorTranslationParser();
	~EditorTranslationParser();
};

#endif // EDITOR_TRANSLATION_PARSER_H
