// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "utilities.h"

using namespace RendererDummy;

Utilities *Utilities::singleton = nullptr;

Utilities::Utilities() {
	singleton = this;
}

Utilities::~Utilities() {
	singleton = nullptr;
}
