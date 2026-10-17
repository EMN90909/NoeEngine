// Copyright (c) 2026 Noethric.
// SPDX-License-Identifier: LicenseRef-Noethric-Proprietary
#ifndef NOEENGINE_NOQERI_TRANSLATOR_H
#define NOEENGINE_NOQERI_TRANSLATOR_H

#include "core/string/ustring.h"

// Intentionally restricted native game-scripting bridge.
// Not a replacement for, or implementation of, the GPL Noqeri compiler.
bool noqeri_translate_game_script(const String &p_source, String &r_compiled, String &r_error);

#endif
