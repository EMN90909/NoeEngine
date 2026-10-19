// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef EXTENSION_API_DUMP_H
#define EXTENSION_API_DUMP_H

#include "core/extension/gdextension.h"

#ifdef TOOLS_ENABLED

class GDExtensionAPIDump {
public:
	static Dictionary generate_extension_api();
	static void generate_extension_json_file(const String &p_path);
};
#endif

#endif // EXTENSION_API_DUMP_H
