// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef EDITOR_EXPORT_SHARED_OBJECT_H
#define EDITOR_EXPORT_SHARED_OBJECT_H

#include "core/string/ustring.h"
#include "core/templates/vector.h"

struct SharedObject {
	String path;
	Vector<String> tags;
	String target;

	SharedObject(const String &p_path, const Vector<String> &p_tags, const String &p_target) :
			path(p_path),
			tags(p_tags),
			target(p_target) {
	}

	SharedObject() {}
};

#endif // EDITOR_EXPORT_SHARED_OBJECT_H
