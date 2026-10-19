// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef CVTT_REGISTER_TYPES_H
#define CVTT_REGISTER_TYPES_H

#ifdef TOOLS_ENABLED

#include "modules/register_module_types.h"

void initialize_cvtt_module(ModuleInitializationLevel p_level);
void uninitialize_cvtt_module(ModuleInitializationLevel p_level);

#endif // TOOLS_ENABLED

#endif // CVTT_REGISTER_TYPES_H
