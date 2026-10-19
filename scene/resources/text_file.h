// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef TEXT_FILE_H
#define TEXT_FILE_H

#include "core/io/resource_loader.h"
#include "core/io/resource_saver.h"

class TextFile : public Resource {
	GDCLASS(TextFile, Resource);

private:
	String text;
	String path;

public:
	virtual bool has_text() const;
	virtual String get_text() const;
	virtual void set_text(const String &p_code);
	virtual void reload_from_file() override;

	void set_file_path(const String &p_path) { path = p_path; }
	Error load_text(const String &p_path);
};

#endif // TEXT_FILE_H
