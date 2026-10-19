// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef MONO_NAMING_UTILS_H
#define MONO_NAMING_UTILS_H

#include "core/string/ustring.h"

String pascal_to_pascal_case(const String &p_identifier);

String snake_to_pascal_case(const String &p_identifier, bool p_input_is_upper = false);

String snake_to_camel_case(const String &p_identifier, bool p_input_is_upper = false);

#endif // MONO_NAMING_UTILS_H
