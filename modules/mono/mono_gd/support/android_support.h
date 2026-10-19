// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef ANDROID_SUPPORT_H
#define ANDROID_SUPPORT_H

#if defined(ANDROID_ENABLED)

#include "core/string/ustring.h"

namespace gdmono {
namespace android {
namespace support {

String get_app_native_lib_dir();

void initialize();
void cleanup();

void register_internal_calls();
} // namespace support
} // namespace android
} // namespace gdmono

#endif // ANDROID_ENABLED

#endif // ANDROID_SUPPORT_H
