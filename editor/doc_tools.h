// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef DOC_TOOLS_H
#define DOC_TOOLS_H

#include "core/doc_data.h"

class DocTools {
public:
	String version;
	HashMap<String, DocData::ClassDoc> class_list;

	static Error erase_classes(const String &p_dir);

	void merge_from(const DocTools &p_data);
	void remove_from(const DocTools &p_data);
	void add_doc(const DocData::ClassDoc &p_class_doc);
	void remove_doc(const String &p_class_name);
	bool has_doc(const String &p_class_name);
	void generate(bool p_basic_types = false);
	Error load_classes(const String &p_dir);
	Error save_classes(const String &p_default_path, const HashMap<String, String> &p_class_path);

	Error _load(Ref<XMLParser> parser);
	Error load_compressed(const uint8_t *p_data, int p_compressed_size, int p_uncompressed_size);
};

#endif // DOC_TOOLS_H
