// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef TEXT_SERVER_FB_REGISTER_TYPES_H
#define TEXT_SERVER_FB_REGISTER_TYPES_H

#ifdef GDEXTENSION
#include <godot_cpp/core/class_db.hpp>
using namespace godot;
#else
#include "modules/register_module_types.h"
#endif

void initialize_text_server_fb_module(ModuleInitializationLevel p_level);
void uninitialize_text_server_fb_module(ModuleInitializationLevel p_level);

#endif // TEXT_SERVER_FB_REGISTER_TYPES_H
