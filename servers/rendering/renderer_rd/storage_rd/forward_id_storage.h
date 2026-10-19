// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef FORWARD_ID_STORAGE_H
#define FORWARD_ID_STORAGE_H

#include "servers/rendering/storage/utilities.h"

class RendererSceneRenderRD;

namespace RendererRD {

typedef int32_t ForwardID;

enum ForwardIDType {
	FORWARD_ID_TYPE_OMNI_LIGHT,
	FORWARD_ID_TYPE_SPOT_LIGHT,
	FORWARD_ID_TYPE_REFLECTION_PROBE,
	FORWARD_ID_TYPE_DECAL,
	FORWARD_ID_MAX,
};

class ForwardIDStorage {
private:
	static ForwardIDStorage *singleton;

public:
	static ForwardIDStorage *get_singleton() { return singleton; }

	ForwardIDStorage();
	virtual ~ForwardIDStorage();

	virtual RendererRD::ForwardID allocate_forward_id(RendererRD::ForwardIDType p_type) { return -1; }
	virtual void free_forward_id(RendererRD::ForwardIDType p_type, RendererRD::ForwardID p_id) {}
	virtual void map_forward_id(RendererRD::ForwardIDType p_type, RendererRD::ForwardID p_id, uint32_t p_index) {}
	virtual bool uses_forward_ids() const { return false; }
};

} // namespace RendererRD

#endif // FORWARD_ID_STORAGE_H
