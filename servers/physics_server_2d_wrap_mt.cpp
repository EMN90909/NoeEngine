// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "physics_server_2d_wrap_mt.h"

#include "core/os/os.h"

void PhysicsServer2DWrapMT::thread_exit() {
	exit.set();
}

void PhysicsServer2DWrapMT::thread_step(real_t p_delta) {
	physics_server_2d->step(p_delta);
	step_sem.post();
}

void PhysicsServer2DWrapMT::_thread_callback(void *_instance) {
	PhysicsServer2DWrapMT *vsmt = reinterpret_cast<PhysicsServer2DWrapMT *>(_instance);

	vsmt->thread_loop();
}

void PhysicsServer2DWrapMT::thread_loop() {
	server_thread = Thread::get_caller_id();

	physics_server_2d->init();

	exit.clear();
	step_thread_up.set();
	while (!exit.is_set()) {
		// flush commands one by one, until exit is requested
		command_queue.wait_and_flush();
	}

	command_queue.flush_all(); // flush all

	physics_server_2d->finish();
}

/* EVENT QUEUING */

void PhysicsServer2DWrapMT::step(real_t p_step) {
	if (create_thread) {
		command_queue.push(this, &PhysicsServer2DWrapMT::thread_step, p_step);
	} else {
		command_queue.flush_all(); //flush all pending from other threads
		physics_server_2d->step(p_step);
	}
}

void PhysicsServer2DWrapMT::sync() {
	if (create_thread) {
		if (first_frame) {
			first_frame = false;
		} else {
			step_sem.wait(); //must not wait if a step was not issued
		}
	}
	physics_server_2d->sync();
}

void PhysicsServer2DWrapMT::flush_queries() {
	physics_server_2d->flush_queries();
}

void PhysicsServer2DWrapMT::end_sync() {
	physics_server_2d->end_sync();
}

void PhysicsServer2DWrapMT::init() {
	if (create_thread) {
		//OS::get_singleton()->release_rendering_thread();
		thread.start(_thread_callback, this);
		while (!step_thread_up.is_set()) {
			OS::get_singleton()->delay_usec(1000);
		}
	} else {
		physics_server_2d->init();
	}
}

void PhysicsServer2DWrapMT::finish() {
	if (thread.is_started()) {
		command_queue.push(this, &PhysicsServer2DWrapMT::thread_exit);
		thread.wait_to_finish();
	} else {
		physics_server_2d->finish();
	}
}

PhysicsServer2DWrapMT::PhysicsServer2DWrapMT(PhysicsServer2D *p_contained, bool p_create_thread) :
		command_queue(p_create_thread) {
	physics_server_2d = p_contained;
	create_thread = p_create_thread;

	pool_max_size = GLOBAL_GET("memory/limits/multithreaded_server/rid_pool_prealloc");

	if (!p_create_thread) {
		server_thread = Thread::get_caller_id();
	} else {
		server_thread = 0;
	}

	main_thread = Thread::get_caller_id();
}

PhysicsServer2DWrapMT::~PhysicsServer2DWrapMT() {
	memdelete(physics_server_2d);
	//finish();
}
