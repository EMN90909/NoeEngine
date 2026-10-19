// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "text_file.h"

#include "core/io/file_access.h"

bool TextFile::has_text() const {
	return !text.is_empty();
}

String TextFile::get_text() const {
	return text;
}

void TextFile::set_text(const String &p_code) {
	text = p_code;
}

void TextFile::reload_from_file() {
	load_text(path);
}

Error TextFile::load_text(const String &p_path) {
	Vector<uint8_t> sourcef;
	Error err;
	Ref<FileAccess> f = FileAccess::open(p_path, FileAccess::READ, &err);

	ERR_FAIL_COND_V_MSG(err, err, "Cannot open TextFile '" + p_path + "'.");

	uint64_t len = f->get_length();
	sourcef.resize(len + 1);
	uint8_t *w = sourcef.ptrw();
	uint64_t r = f->get_buffer(w, len);

	ERR_FAIL_COND_V(r != len, ERR_CANT_OPEN);
	w[len] = 0;

	String s;
	ERR_FAIL_COND_V_MSG(s.parse_utf8((const char *)w) != OK, ERR_INVALID_DATA, "Script '" + p_path + "' contains invalid unicode (UTF-8), so it was not loaded. Please ensure that scripts are saved in valid UTF-8 unicode.");
	text = s;
	path = p_path;
#ifdef TOOLS_ENABLED
	if (ResourceLoader::get_timestamp_on_load()) {
		set_last_modified_time(FileAccess::get_modified_time(path));
	}
#endif // TOOLS_ENABLED
	return OK;
}
