// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef IOS_SUPPORT_H
#define IOS_SUPPORT_H

#if defined(IOS_ENABLED)

#include "core/string/ustring.h"

namespace gdmono {
namespace ios {
namespace support {

void initialize();
void cleanup();
} // namespace support
} // namespace ios
} // namespace gdmono

#endif // IOS_ENABLED

#endif // IOS_SUPPORT_H
