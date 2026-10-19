// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef RUNTIME_INTEROP_H
#define RUNTIME_INTEROP_H

#include "core/typedefs.h"

namespace godotsharp {
const void **get_runtime_interop_funcs(int32_t &r_size);
}

#endif // RUNTIME_INTEROP_H
