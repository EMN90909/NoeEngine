// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef OPENXR_REGISTER_TYPES_H
#define OPENXR_REGISTER_TYPES_H

#define MODULE_OPENXR_HAS_PREREGISTER

#include "modules/register_module_types.h"

void initialize_openxr_module(ModuleInitializationLevel p_level);
void uninitialize_openxr_module(ModuleInitializationLevel p_level);

#endif // OPENXR_REGISTER_TYPES_H
