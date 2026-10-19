// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef RENDERER_FOG_H
#define RENDERER_FOG_H

#include "servers/rendering_server.h"

class RendererFog {
public:
	virtual ~RendererFog() {}

	/* FOG VOLUMES */

	virtual RID fog_volume_allocate() = 0;
	virtual void fog_volume_initialize(RID p_rid) = 0;
	virtual void fog_volume_free(RID p_rid) = 0;

	virtual void fog_volume_set_shape(RID p_fog_volume, RS::FogVolumeShape p_shape) = 0;
	virtual void fog_volume_set_size(RID p_fog_volume, const Vector3 &p_size) = 0;
	virtual void fog_volume_set_material(RID p_fog_volume, RID p_material) = 0;
	virtual AABB fog_volume_get_aabb(RID p_fog_volume) const = 0;
	virtual RS::FogVolumeShape fog_volume_get_shape(RID p_fog_volume) const = 0;
};

#endif // RENDERER_FOG_H
