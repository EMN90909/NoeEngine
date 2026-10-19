// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifdef GDEXTENSION
// Headers for building as GDExtension plug-in.

#include <godot_cpp/godot.hpp>

using namespace godot;

#else
// Headers for building as built-in module.

#include "core/typedefs.h"

#include "modules/modules_enabled.gen.h" // For svg.
#endif

#ifdef MODULE_SVG_ENABLED

#include "thorvg_bounds_iterator.h"

#include <tvgIteratorAccessor.h>
#include <tvgPaint.h>

// This function uses private ThorVG API to get bounding box of top level children elements.

void tvg_get_bounds(tvg::Picture *p_picture, float &r_min_x, float &r_min_y, float &r_max_x, float &r_max_y) {
	tvg::IteratorAccessor itrAccessor;
	if (tvg::Iterator *it = itrAccessor.iterator(p_picture)) {
		while (const tvg::Paint *child = it->next()) {
			float x = 0, y = 0, w = 0, h = 0;
			child->bounds(&x, &y, &w, &h, true);
			r_min_x = MIN(x, r_min_x);
			r_min_y = MIN(y, r_min_y);
			r_max_x = MAX(x + w, r_max_x);
			r_max_y = MAX(y + h, r_max_y);
		}
		delete (it);
	}
}

#endif // MODULE_SVG_ENABLED
