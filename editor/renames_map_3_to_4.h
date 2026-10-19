// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef RENAMES_MAP_3_TO_4_H
#define RENAMES_MAP_3_TO_4_H

#ifndef DISABLE_DEPRECATED

struct RenamesMap3To4 {
	static const char *enum_renames[][2];
	static const char *gdscript_function_renames[][2];
	static const char *csharp_function_renames[][2];
	static const char *gdscript_properties_renames[][2];
	static const char *csharp_properties_renames[][2];
	static const char *gdscript_signals_renames[][2];
	static const char *csharp_signals_renames[][2];
	static const char *project_settings_renames[][2];
	static const char *project_godot_renames[][2];
	static const char *input_map_renames[][2];
	static const char *builtin_types_renames[][2];
	static const char *shaders_renames[][2];
	static const char *class_renames[][2];
	static const char *color_renames[][2];
	static const char *theme_override_renames[][2];
};

#endif // DISABLE_DEPRECATED

#endif // RENAMES_MAP_3_TO_4_H
