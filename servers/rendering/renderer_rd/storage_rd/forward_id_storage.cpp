// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "forward_id_storage.h"

using namespace RendererRD;

ForwardIDStorage *ForwardIDStorage::singleton = nullptr;

ForwardIDStorage::ForwardIDStorage() {
	singleton = this;
}

ForwardIDStorage::~ForwardIDStorage() {
	singleton = nullptr;
}
