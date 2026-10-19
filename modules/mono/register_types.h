// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef MONO_REGISTER_TYPES_H
#define MONO_REGISTER_TYPES_H

#include "modules/register_module_types.h"

void initialize_mono_module(ModuleInitializationLevel p_level);
void uninitialize_mono_module(ModuleInitializationLevel p_level);

#endif // MONO_REGISTER_TYPES_H
