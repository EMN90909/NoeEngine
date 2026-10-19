// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef GODOTSHARP_DIRS_H
#define GODOTSHARP_DIRS_H

#include "core/string/ustring.h"

namespace GodotSharpDirs {

String get_res_metadata_dir();
String get_res_temp_assemblies_dir();

String get_api_assemblies_dir();

String get_mono_user_dir();

#ifdef TOOLS_ENABLED
String get_build_logs_dir();
String get_data_editor_tools_dir();
#endif

} // namespace GodotSharpDirs

#endif // GODOTSHARP_DIRS_H
