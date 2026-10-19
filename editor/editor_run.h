// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef EDITOR_RUN_H
#define EDITOR_RUN_H

#include "core/os/os.h"

class EditorRun {
public:
	enum Status {
		STATUS_PLAY,
		STATUS_PAUSED,
		STATUS_STOP
	};

	List<OS::ProcessID> pids;

private:
	Status status;
	String running_scene;

public:
	Status get_status() const;
	String get_running_scene() const;
	Error run(const String &p_scene, const String &p_write_movie = "");
	void run_native_notify() { status = STATUS_PLAY; }
	void stop();

	void stop_child_process(OS::ProcessID p_pid);
	bool has_child_process(OS::ProcessID p_pid) const;
	int get_child_process_count() const { return pids.size(); }

	EditorRun();
};

#endif // EDITOR_RUN_H
