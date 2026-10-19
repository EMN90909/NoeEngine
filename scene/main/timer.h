// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef TIMER_H
#define TIMER_H

#include "scene/main/node.h"

class Timer : public Node {
	GDCLASS(Timer, Node);

	double wait_time = 1.0;
	bool one_shot = false;
	bool autostart = false;
	bool processing = false;
	bool paused = false;

	double time_left = -1.0;

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	enum TimerProcessCallback {
		TIMER_PROCESS_PHYSICS,
		TIMER_PROCESS_IDLE,
	};

	void set_wait_time(double p_time);
	double get_wait_time() const;

	void set_one_shot(bool p_one_shot);
	bool is_one_shot() const;

	void set_autostart(bool p_start);
	bool has_autostart() const;

	void start(double p_time = -1);
	void stop();

	void set_paused(bool p_paused);
	bool is_paused() const;

	bool is_stopped() const;

	double get_time_left() const;

	PackedStringArray get_configuration_warnings() const override;

	void set_timer_process_callback(TimerProcessCallback p_callback);
	TimerProcessCallback get_timer_process_callback() const;
	Timer();

private:
	TimerProcessCallback timer_process_callback = TIMER_PROCESS_IDLE;
	void _set_process(bool p_process, bool p_force = false);
};

VARIANT_ENUM_CAST(Timer::TimerProcessCallback);

#endif // TIMER_H
