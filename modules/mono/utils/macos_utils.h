// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef MONO_MACOS_UTILS_H
#define MONO_MACOS_UTILS_H

#ifdef MACOS_ENABLED

#include "core/string/ustring.h"

bool macos_is_app_bundle_installed(const String &p_bundle_id);

#endif

#endif // MONO_MACOS_UTILS_H
