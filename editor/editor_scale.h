// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef EDITOR_SCALE_H
#define EDITOR_SCALE_H

void editor_set_scale(float p_scale);
float editor_get_scale();

#define EDSCALE (editor_get_scale())

#endif // EDITOR_SCALE_H
