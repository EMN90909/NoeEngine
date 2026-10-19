// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef ENGINE_PROFILER_H
#define ENGINE_PROFILER_H

#include "core/object/ref_counted.h"

#include "core/object/gdvirtual.gen.inc"
#include "core/object/script_language.h"

class EngineProfiler : public RefCounted {
	GDCLASS(EngineProfiler, RefCounted);

private:
	String registration;

protected:
	static void _bind_methods();

public:
	virtual void toggle(bool p_enable, const Array &p_opts);
	virtual void add(const Array &p_data);
	virtual void tick(double p_frame_time, double p_process_time, double p_physics_time, double p_physics_frame_time);

	Error bind(const String &p_name);
	Error unbind();
	bool is_bound() const { return registration.length() > 0; }

	GDVIRTUAL2(_toggle, bool, Array);
	GDVIRTUAL1(_add_frame, Array);
	GDVIRTUAL4(_tick, double, double, double, double);

	EngineProfiler() {}
	virtual ~EngineProfiler();
};

#endif // ENGINE_PROFILER_H
