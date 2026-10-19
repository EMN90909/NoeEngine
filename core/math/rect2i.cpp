// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "rect2i.h"

#include "core/math/rect2.h"
#include "core/string/ustring.h"

Rect2i::operator String() const {
	return "[P: " + position.operator String() + ", S: " + size + "]";
}

Rect2i::operator Rect2() const {
	return Rect2(position, size);
}
