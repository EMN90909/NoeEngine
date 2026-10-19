// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef CLASS_DB_API_JSON_H
#define CLASS_DB_API_JSON_H

// 'core/object/method_bind.h' defines DEBUG_METHODS_ENABLED, but it looks like we
// cannot include it here. That's why we include it through 'core/object/class_db.h'.
#include "core/object/class_db.h"

#ifdef DEBUG_METHODS_ENABLED

#include "core/string/ustring.h"

void class_db_api_to_json(const String &p_output_file, ClassDB::APIType p_api);

#endif // DEBUG_METHODS_ENABLED

#endif // CLASS_DB_API_JSON_H
