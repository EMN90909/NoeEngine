// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef AUDIO_LISTENER_3D_H
#define AUDIO_LISTENER_3D_H

#include "scene/3d/node_3d.h"

class AudioListener3D : public Node3D {
	GDCLASS(AudioListener3D, Node3D);

private:
	bool force_change = false;
	bool current = false;

	RID scenario_id;

	friend class Viewport;
	void _update_audio_listener_state();

protected:
	void _update_listener();
	virtual void _request_listener_update();

	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;
	void _notification(int p_what);

	static void _bind_methods();

public:
	void make_current();
	void clear_current();
	bool is_current() const;

	virtual Transform3D get_listener_transform() const;

	AudioListener3D();
	~AudioListener3D();
};

#endif // AUDIO_LISTENER_3D_H
