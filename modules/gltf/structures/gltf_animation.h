// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef GLTF_ANIMATION_H
#define GLTF_ANIMATION_H

#include "scene/animation/animation_player.h"

class GLTFAnimation : public Resource {
	GDCLASS(GLTFAnimation, Resource);

protected:
	static void _bind_methods();

public:
	enum Interpolation {
		INTERP_LINEAR,
		INTERP_STEP,
		INTERP_CATMULLROMSPLINE,
		INTERP_CUBIC_SPLINE,
	};

	template <class T>
	struct Channel {
		Interpolation interpolation;
		Vector<real_t> times;
		Vector<T> values;
	};

	struct Track {
		Channel<Vector3> position_track;
		Channel<Quaternion> rotation_track;
		Channel<Vector3> scale_track;
		Vector<Channel<real_t>> weight_tracks;
	};

public:
	bool get_loop() const;
	void set_loop(bool p_val);
	HashMap<int, GLTFAnimation::Track> &get_tracks();
	GLTFAnimation();

private:
	bool loop = false;
	HashMap<int, Track> tracks;
};

#endif // GLTF_ANIMATION_H
