// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef EDITOR_TRANSLATION_H
#define EDITOR_TRANSLATION_H

#include "core/string/ustring.h"
#include "core/templates/vector.h"

Vector<String> get_editor_locales();
void load_editor_translations(const String &p_locale);
void load_doc_translations(const String &p_locale);
void load_property_translations(const String &p_locale);

#endif // EDITOR_TRANSLATION_H
