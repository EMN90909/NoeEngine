// Copyright (c) 2026 Noethric.
// SPDX-License-Identifier: LicenseRef-Noethric-Proprietary
#ifndef NOEENGINE_NOQERI_TRANSLATOR_H
#define NOEENGINE_NOQERI_TRANSLATOR_H

#include "core/string/ustring.h"
#include "core/templates/vector.h"

// Intentionally restricted native game-scripting bridge.
// Not a replacement for, or implementation of, the GPL Noqeri compiler.
// r_line_map maps translated GDScript lines (index 0 = line 1) to original
// Noqeri source lines. r_error_line is 1-based for parser failures.
bool noqeri_translate_game_script(const String &p_source, String &r_compiled,
        String &r_error, Vector<int> *r_line_map = nullptr,
        int *r_error_line = nullptr);

#endif
