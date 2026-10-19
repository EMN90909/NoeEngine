// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef RENDER_BUFFER_CUSTOM_DATA_RD_H
#define RENDER_BUFFER_CUSTOM_DATA_RD_H

#include "core/object/ref_counted.h"

class RenderSceneBuffersRD;

class RenderBufferCustomDataRD : public RefCounted {
	GDCLASS(RenderBufferCustomDataRD, RefCounted);

public:
	virtual void configure(RenderSceneBuffersRD *p_render_buffers) = 0;
	virtual void free_data() = 0; // called on cleanup

private:
};

#endif // RENDER_BUFFER_CUSTOM_DATA_RD_H
