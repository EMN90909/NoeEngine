// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "register_types.h"

#include "image_compress_etcpak.h"

void initialize_etcpak_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	Image::_image_compress_etc1_func = _compress_etc1;
	Image::_image_compress_etc2_func = _compress_etc2;
	Image::_image_compress_bc_func = _compress_bc;
}

void uninitialize_etcpak_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}
