// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef REGEX_H
#define REGEX_H

#include "core/object/ref_counted.h"
#include "core/string/ustring.h"
#include "core/templates/rb_map.h"
#include "core/templates/vector.h"
#include "core/variant/array.h"
#include "core/variant/dictionary.h"
#include "core/variant/typed_array.h"

class RegExMatch : public RefCounted {
	GDCLASS(RegExMatch, RefCounted);

	struct Range {
		int start = 0;
		int end = 0;
	};

	String subject;
	Vector<Range> data;
	HashMap<String, int> names;

	friend class RegEx;

protected:
	static void _bind_methods();

	int _find(const Variant &p_name) const;

public:
	String get_subject() const;
	int get_group_count() const;
	Dictionary get_names() const;

	PackedStringArray get_strings() const;
	String get_string(const Variant &p_name) const;
	int get_start(const Variant &p_name) const;
	int get_end(const Variant &p_name) const;
};

class RegEx : public RefCounted {
	GDCLASS(RegEx, RefCounted);

	void *general_ctx = nullptr;
	void *code = nullptr;
	String pattern;

	void _pattern_info(uint32_t what, void *where) const;

protected:
	static void _bind_methods();

public:
	static Ref<RegEx> create_from_string(const String &p_pattern);

	void clear();
	Error compile(const String &p_pattern);

	Ref<RegExMatch> search(const String &p_subject, int p_offset = 0, int p_end = -1) const;
	TypedArray<RegExMatch> search_all(const String &p_subject, int p_offset = 0, int p_end = -1) const;
	String sub(const String &p_subject, const String &p_replacement, bool p_all = false, int p_offset = 0, int p_end = -1) const;

	bool is_valid() const;
	String get_pattern() const;
	int get_group_count() const;
	PackedStringArray get_names() const;

	RegEx();
	RegEx(const String &p_pattern);
	~RegEx();
};

#endif // REGEX_H
