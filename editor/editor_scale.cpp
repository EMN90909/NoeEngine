// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "editor_scale.h"

#include "core/os/os.h"

static float scale = 1.0;

void editor_set_scale(float p_scale) {
	scale = p_scale;
}

float editor_get_scale() {
	return scale;
}
