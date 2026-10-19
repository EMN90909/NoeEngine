// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "random_pcg.h"

#include "core/os/os.h"

RandomPCG::RandomPCG(uint64_t p_seed, uint64_t p_inc) :
		pcg(),
		current_inc(p_inc) {
	seed(p_seed);
}

void RandomPCG::randomize() {
	seed(((uint64_t)OS::get_singleton()->get_unix_time() + OS::get_singleton()->get_ticks_usec()) * pcg.state + PCG_DEFAULT_INC_64);
}

double RandomPCG::random(double p_from, double p_to) {
	return randd() * (p_to - p_from) + p_from;
}

float RandomPCG::random(float p_from, float p_to) {
	return randf() * (p_to - p_from) + p_from;
}

int RandomPCG::random(int p_from, int p_to) {
	if (p_from == p_to) {
		return p_from;
	}
	return rand(abs(p_from - p_to) + 1) + MIN(p_from, p_to);
}
