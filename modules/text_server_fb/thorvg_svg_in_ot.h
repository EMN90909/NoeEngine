// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef THORVG_SVG_IN_OT_H
#define THORVG_SVG_IN_OT_H

#ifdef GDEXTENSION
// Headers for building as GDExtension plug-in.

#include <godot_cpp/core/mutex_lock.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/templates/hash_map.hpp>

using namespace godot;

#else
// Headers for building as built-in module.

#include "core/os/mutex.h"
#include "core/templates/hash_map.h"
#include "core/typedefs.h"

#include "modules/modules_enabled.gen.h" // For svg.
#endif

#ifdef MODULE_SVG_ENABLED

#include <freetype/freetype.h>
#include <freetype/otsvg.h>
#include <ft2build.h>
#include <thorvg.h>

struct GL_State {
	bool ready = false;
	float bmp_x = 0;
	float bmp_y = 0;
	float x = 0;
	float y = 0;
	float w = 0;
	float h = 0;
	CharString xml_code;
	tvg::Matrix m;
};

struct TVG_State {
	Mutex mutex;
	HashMap<uint32_t, GL_State> glyph_map;
};

FT_Error tvg_svg_in_ot_init(FT_Pointer *p_state);
void tvg_svg_in_ot_free(FT_Pointer *p_state);
FT_Error tvg_svg_in_ot_preset_slot(FT_GlyphSlot p_slot, FT_Bool p_cache, FT_Pointer *p_state);
FT_Error tvg_svg_in_ot_render(FT_GlyphSlot p_slot, FT_Pointer *p_state);

SVG_RendererHooks *get_tvg_svg_in_ot_hooks();

#endif // MODULE_SVG_ENABLED

#endif // THORVG_SVG_IN_OT_H
