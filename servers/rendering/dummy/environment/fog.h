// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef FOG_DUMMY_H
#define FOG_DUMMY_H

#include "servers/rendering/environment/renderer_fog.h"

namespace RendererDummy {

class Fog : public RendererFog {
public:
	/* FOG VOLUMES */

	virtual RID fog_volume_allocate() override { return RID(); }
	virtual void fog_volume_initialize(RID p_rid) override {}
	virtual void fog_volume_free(RID p_rid) override {}

	virtual void fog_volume_set_shape(RID p_fog_volume, RS::FogVolumeShape p_shape) override {}
	virtual void fog_volume_set_size(RID p_fog_volume, const Vector3 &p_size) override {}
	virtual void fog_volume_set_material(RID p_fog_volume, RID p_material) override {}
	virtual AABB fog_volume_get_aabb(RID p_fog_volume) const override { return AABB(); }
	virtual RS::FogVolumeShape fog_volume_get_shape(RID p_fog_volume) const override { return RS::FOG_VOLUME_SHAPE_BOX; }
};

} // namespace RendererDummy

#endif // FOG_DUMMY_H
