// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef MAIN_TIMER_SYNC_H
#define MAIN_TIMER_SYNC_H

#include "core/config/engine.h"

struct MainFrameTime {
	double process_step; // delta time to advance during process()
	int physics_steps; // number of times to iterate the physics engine
	double interpolation_fraction; // fraction through the current physics tick

	void clamp_process_step(double min_process_step, double max_process_step);
};

class MainTimerSync {
	// wall clock time measured on the main thread
	uint64_t last_cpu_ticks_usec = 0;
	uint64_t current_cpu_ticks_usec = 0;

	// logical game time since last physics timestep
	double time_accum = 0;

	// current difference between wall clock time and reported sum of process_steps
	double time_deficit = 0;

	// number of frames back for keeping accumulated physics steps roughly constant.
	// value of 12 chosen because that is what is required to make 144 Hz monitors
	// behave well with 60 Hz physics updates. The only worse commonly available refresh
	// would be 85, requiring CONTROL_STEPS = 17.
	static const int CONTROL_STEPS = 12;

	// sum of physics steps done over the last (i+1) frames
	int accumulated_physics_steps[CONTROL_STEPS];

	// typical value for accumulated_physics_steps[i] is either this or this plus one
	int typical_physics_steps[CONTROL_STEPS];

	int fixed_fps = 0;

protected:
	// returns the fraction of p_physics_step required for the timer to overshoot
	// before advance_core considers changing the physics_steps return from
	// the typical values as defined by typical_physics_steps
	double get_physics_jitter_fix();

	// gets our best bet for the average number of physics steps per render frame
	// return value: number of frames back this data is consistent
	int get_average_physics_steps(double &p_min, double &p_max);

	// advance physics clock by p_process_step, return appropriate number of steps to simulate
	MainFrameTime advance_core(double p_physics_step, int p_physics_ticks_per_second, double p_process_step);

	// calls advance_core, keeps track of deficit it adds to animaption_step, make sure the deficit sum stays close to zero
	MainFrameTime advance_checked(double p_physics_step, int p_physics_ticks_per_second, double p_process_step);

	// determine wall clock step since last iteration
	double get_cpu_process_step();

public:
	MainTimerSync();

	// start the clock
	void init(uint64_t p_cpu_ticks_usec);
	// set measured wall clock time
	void set_cpu_ticks_usec(uint64_t p_cpu_ticks_usec);
	//set fixed fps
	void set_fixed_fps(int p_fixed_fps);

	// advance one frame, return timesteps to take
	MainFrameTime advance(double p_physics_step, int p_physics_ticks_per_second);
};

#endif // MAIN_TIMER_SYNC_H
