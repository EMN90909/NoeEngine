// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "texture_storage.h"

using namespace RendererDummy;

TextureStorage *TextureStorage::singleton = nullptr;

TextureStorage::TextureStorage() {
	singleton = this;
}

TextureStorage::~TextureStorage() {
	singleton = nullptr;
}
