// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef RANDOM_NUMBER_GENERATOR_H
#define RANDOM_NUMBER_GENERATOR_H

#include "core/math/random_pcg.h"
#include "core/object/ref_counted.h"

class RandomNumberGenerator : public RefCounted {
	GDCLASS(RandomNumberGenerator, RefCounted);

protected:
	RandomPCG randbase;

	static void _bind_methods();

public:
	_FORCE_INLINE_ void set_seed(uint64_t p_seed) { randbase.seed(p_seed); }
	_FORCE_INLINE_ uint64_t get_seed() { return randbase.get_seed(); }

	_FORCE_INLINE_ void set_state(uint64_t p_state) { randbase.set_state(p_state); }
	_FORCE_INLINE_ uint64_t get_state() const { return randbase.get_state(); }

	_FORCE_INLINE_ void randomize() { randbase.randomize(); }

	_FORCE_INLINE_ uint32_t randi() { return randbase.rand(); }
	_FORCE_INLINE_ real_t randf() { return randbase.randf(); }
	_FORCE_INLINE_ real_t randf_range(real_t p_from, real_t p_to) { return randbase.random(p_from, p_to); }
	_FORCE_INLINE_ real_t randfn(real_t p_mean = 0.0, real_t p_deviation = 1.0) { return randbase.randfn(p_mean, p_deviation); }
	_FORCE_INLINE_ int randi_range(int p_from, int p_to) { return randbase.random(p_from, p_to); }

	RandomNumberGenerator() { randbase.randomize(); }
};

#endif // RANDOM_NUMBER_GENERATOR_H
