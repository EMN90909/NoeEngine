// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef VELOCITY_TRACKER_3D_H
#define VELOCITY_TRACKER_3D_H

#include "scene/3d/node_3d.h"

class VelocityTracker3D : public RefCounted {
	struct PositionHistory {
		uint64_t frame = 0;
		Vector3 position;
	};

	bool physics_step = false;
	Vector<PositionHistory> position_history;
	int position_history_len = 0;

protected:
	static void _bind_methods();

public:
	void reset(const Vector3 &p_new_pos);
	void set_track_physics_step(bool p_track_physics_step);
	bool is_tracking_physics_step() const;
	void update_position(const Vector3 &p_position);
	Vector3 get_tracked_linear_velocity() const;

	VelocityTracker3D();
};

#endif // VELOCITY_TRACKER_3D_H
