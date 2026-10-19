// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef PHYSICS_MATERIAL_H
#define PHYSICS_MATERIAL_H

#include "core/io/resource.h"
#include "servers/physics_server_3d.h"

class PhysicsMaterial : public Resource {
	GDCLASS(PhysicsMaterial, Resource);
	OBJ_SAVE_TYPE(PhysicsMaterial);
	RES_BASE_EXTENSION("phymat");

	real_t friction = 1.0;
	bool rough = false;
	real_t bounce = 0.0;
	bool absorbent = false;

protected:
	static void _bind_methods();

public:
	void set_friction(real_t p_val);
	_FORCE_INLINE_ real_t get_friction() const { return friction; }

	void set_rough(bool p_val);
	_FORCE_INLINE_ bool is_rough() const { return rough; }

	_FORCE_INLINE_ real_t computed_friction() const {
		return rough ? -friction : friction;
	}

	void set_bounce(real_t p_val);
	_FORCE_INLINE_ real_t get_bounce() const { return bounce; }

	void set_absorbent(bool p_val);
	_FORCE_INLINE_ bool is_absorbent() const { return absorbent; }

	_FORCE_INLINE_ real_t computed_bounce() const {
		return absorbent ? -bounce : bounce;
	}

	PhysicsMaterial() {}
};

#endif // PHYSICS_MATERIAL_H
