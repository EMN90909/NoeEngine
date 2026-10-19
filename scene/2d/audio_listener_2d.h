// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef AUDIO_LISTENER_2D_H
#define AUDIO_LISTENER_2D_H

#include "scene/2d/node_2d.h"
#include "scene/main/window.h"

class AudioListener2D : public Node2D {
	GDCLASS(AudioListener2D, Node2D);

private:
	bool current = false;

	friend class Viewport;

protected:
	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;
	void _notification(int p_what);

	static void _bind_methods();

public:
	void make_current();
	void clear_current();
	bool is_current() const;

	AudioListener2D();
};

#endif // AUDIO_LISTENER_2D_H
