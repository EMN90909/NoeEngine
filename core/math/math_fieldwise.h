// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef MATH_FIELDWISE_H
#define MATH_FIELDWISE_H

#ifdef TOOLS_ENABLED

#include "core/variant/variant.h"

Variant fieldwise_assign(const Variant &p_target, const Variant &p_source, const String &p_field);

#endif // TOOLS_ENABLED

#endif // MATH_FIELDWISE_H
