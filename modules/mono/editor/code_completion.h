// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef CODE_COMPLETION_H
#define CODE_COMPLETION_H

#include "core/string/ustring.h"
#include "core/variant/variant.h"

namespace gdmono {

enum class CompletionKind {
	INPUT_ACTIONS = 0,
	NODE_PATHS,
	RESOURCE_PATHS,
	SCENE_PATHS,
	SHADER_PARAMS,
	SIGNALS,
	THEME_COLORS,
	THEME_CONSTANTS,
	THEME_FONTS,
	THEME_FONT_SIZES,
	THEME_STYLES
};

PackedStringArray get_code_completion(CompletionKind p_kind, const String &p_script_file);
} // namespace gdmono

#endif // CODE_COMPLETION_H
