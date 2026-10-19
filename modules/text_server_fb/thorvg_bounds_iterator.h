// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef THORVG_BOUNDS_ITERATOR_H
#define THORVG_BOUNDS_ITERATOR_H

#ifdef GDEXTENSION
// Headers for building as GDExtension plug-in.

#include <godot_cpp/core/mutex_lock.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

#else
// Headers for building as built-in module.

#include "core/typedefs.h"

#include "modules/modules_enabled.gen.h" // For svg.
#endif

#ifdef MODULE_SVG_ENABLED

#include <thorvg.h>

void tvg_get_bounds(tvg::Picture *p_picture, float &r_min_x, float &r_min_y, float &r_max_x, float &r_max_y);

#endif // MODULE_SVG_ENABLED

#endif // THORVG_BOUNDS_ITERATOR_H
