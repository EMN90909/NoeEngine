// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef SHORTCUT_H
#define SHORTCUT_H

#include "core/input/input_event.h"
#include "core/io/resource.h"

class Shortcut : public Resource {
	GDCLASS(Shortcut, Resource);

	Array events;

protected:
	static void _bind_methods();

public:
	void set_events(const Array &p_events);
	Array get_events() const;

	void set_events_list(const List<Ref<InputEvent>> *p_events);

	bool matches_event(const Ref<InputEvent> &p_event) const;
	bool has_valid_event() const;

	String get_as_text() const;

	static bool is_event_array_equal(const Array &p_event_array1, const Array &p_event_array2);
};

#endif // SHORTCUT_H
