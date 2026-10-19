// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "command_queue_mt.h"

#include "core/config/project_settings.h"
#include "core/os/os.h"

void CommandQueueMT::lock() {
	mutex.lock();
}

void CommandQueueMT::unlock() {
	mutex.unlock();
}

void CommandQueueMT::wait_for_flush() {
	// wait one millisecond for a flush to happen
	OS::get_singleton()->delay_usec(1000);
}

CommandQueueMT::SyncSemaphore *CommandQueueMT::_alloc_sync_sem() {
	int idx = -1;

	while (true) {
		lock();
		for (int i = 0; i < SYNC_SEMAPHORES; i++) {
			if (!sync_sems[i].in_use) {
				sync_sems[i].in_use = true;
				idx = i;
				break;
			}
		}
		unlock();

		if (idx == -1) {
			wait_for_flush();
		} else {
			break;
		}
	}

	return &sync_sems[idx];
}

CommandQueueMT::CommandQueueMT(bool p_sync) {
	if (p_sync) {
		sync = memnew(Semaphore);
	}
}

CommandQueueMT::~CommandQueueMT() {
	if (sync) {
		memdelete(sync);
	}
}
