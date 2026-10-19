// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef GODOT_CONSTRAINT_2D_H
#define GODOT_CONSTRAINT_2D_H

#include "godot_body_2d.h"

class GodotConstraint2D {
	GodotBody2D **_body_ptr;
	int _body_count;
	uint64_t island_step = 0;
	bool disabled_collisions_between_bodies = true;

	RID self;

protected:
	GodotConstraint2D(GodotBody2D **p_body_ptr = nullptr, int p_body_count = 0) {
		_body_ptr = p_body_ptr;
		_body_count = p_body_count;
	}

public:
	_FORCE_INLINE_ void set_self(const RID &p_self) { self = p_self; }
	_FORCE_INLINE_ RID get_self() const { return self; }

	_FORCE_INLINE_ uint64_t get_island_step() const { return island_step; }
	_FORCE_INLINE_ void set_island_step(uint64_t p_step) { island_step = p_step; }

	_FORCE_INLINE_ GodotBody2D **get_body_ptr() const { return _body_ptr; }
	_FORCE_INLINE_ int get_body_count() const { return _body_count; }

	_FORCE_INLINE_ void disable_collisions_between_bodies(const bool p_disabled) { disabled_collisions_between_bodies = p_disabled; }
	_FORCE_INLINE_ bool is_disabled_collisions_between_bodies() const { return disabled_collisions_between_bodies; }

	virtual bool setup(real_t p_step) = 0;
	virtual bool pre_solve(real_t p_step) = 0;
	virtual void solve(real_t p_step) = 0;

	virtual ~GodotConstraint2D() {}
};

#endif // GODOT_CONSTRAINT_2D_H
