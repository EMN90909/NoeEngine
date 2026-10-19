// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef REGISTER_MODULE_TYPES_H
#define REGISTER_MODULE_TYPES_H

#include "core/extension/gdextension_interface.h"

enum ModuleInitializationLevel {
	MODULE_INITIALIZATION_LEVEL_CORE = GDEXTENSION_INITIALIZATION_CORE,
	MODULE_INITIALIZATION_LEVEL_SERVERS = GDEXTENSION_INITIALIZATION_SERVERS,
	MODULE_INITIALIZATION_LEVEL_SCENE = GDEXTENSION_INITIALIZATION_SCENE,
	MODULE_INITIALIZATION_LEVEL_EDITOR = GDEXTENSION_INITIALIZATION_EDITOR
};

void initialize_modules(ModuleInitializationLevel p_level);
void uninitialize_modules(ModuleInitializationLevel p_level);

#endif // REGISTER_MODULE_TYPES_H
