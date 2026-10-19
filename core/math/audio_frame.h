// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef AUDIO_FRAME_H
#define AUDIO_FRAME_H

#include "core/math/vector2.h"
#include "core/typedefs.h"

static inline float undenormalize(volatile float f) {
	union {
		uint32_t i;
		float f;
	} v;

	v.f = f;

	// original: return (v.i & 0x7f800000) == 0 ? 0.0f : f;
	// version from Tim Blechmann:
	return (v.i & 0x7f800000) < 0x08000000 ? 0.0f : f;
}

static const float AUDIO_PEAK_OFFSET = 0.0000000001f;
static const float AUDIO_MIN_PEAK_DB = -200.0f; // linear_to_db(AUDIO_PEAK_OFFSET)

struct AudioFrame {
	//left and right samples
	float l = 0.f, r = 0.f;

	_ALWAYS_INLINE_ const float &operator[](int idx) const { return idx == 0 ? l : r; }
	_ALWAYS_INLINE_ float &operator[](int idx) { return idx == 0 ? l : r; }

	_ALWAYS_INLINE_ AudioFrame operator+(const AudioFrame &p_frame) const { return AudioFrame(l + p_frame.l, r + p_frame.r); }
	_ALWAYS_INLINE_ AudioFrame operator-(const AudioFrame &p_frame) const { return AudioFrame(l - p_frame.l, r - p_frame.r); }
	_ALWAYS_INLINE_ AudioFrame operator*(const AudioFrame &p_frame) const { return AudioFrame(l * p_frame.l, r * p_frame.r); }
	_ALWAYS_INLINE_ AudioFrame operator/(const AudioFrame &p_frame) const { return AudioFrame(l / p_frame.l, r / p_frame.r); }

	_ALWAYS_INLINE_ AudioFrame operator+(float p_sample) const { return AudioFrame(l + p_sample, r + p_sample); }
	_ALWAYS_INLINE_ AudioFrame operator-(float p_sample) const { return AudioFrame(l - p_sample, r - p_sample); }
	_ALWAYS_INLINE_ AudioFrame operator*(float p_sample) const { return AudioFrame(l * p_sample, r * p_sample); }
	_ALWAYS_INLINE_ AudioFrame operator/(float p_sample) const { return AudioFrame(l / p_sample, r / p_sample); }

	_ALWAYS_INLINE_ void operator+=(const AudioFrame &p_frame) {
		l += p_frame.l;
		r += p_frame.r;
	}
	_ALWAYS_INLINE_ void operator-=(const AudioFrame &p_frame) {
		l -= p_frame.l;
		r -= p_frame.r;
	}
	_ALWAYS_INLINE_ void operator*=(const AudioFrame &p_frame) {
		l *= p_frame.l;
		r *= p_frame.r;
	}
	_ALWAYS_INLINE_ void operator/=(const AudioFrame &p_frame) {
		l /= p_frame.l;
		r /= p_frame.r;
	}

	_ALWAYS_INLINE_ void operator+=(float p_sample) {
		l += p_sample;
		r += p_sample;
	}
	_ALWAYS_INLINE_ void operator-=(float p_sample) {
		l -= p_sample;
		r -= p_sample;
	}
	_ALWAYS_INLINE_ void operator*=(float p_sample) {
		l *= p_sample;
		r *= p_sample;
	}
	_ALWAYS_INLINE_ void operator/=(float p_sample) {
		l /= p_sample;
		r /= p_sample;
	}

	_ALWAYS_INLINE_ void undenormalize() {
		l = ::undenormalize(l);
		r = ::undenormalize(r);
	}

	_FORCE_INLINE_ AudioFrame lerp(const AudioFrame &p_b, float p_t) const {
		AudioFrame res = *this;

		res.l += (p_t * (p_b.l - l));
		res.r += (p_t * (p_b.r - r));

		return res;
	}

	_ALWAYS_INLINE_ AudioFrame(float p_l, float p_r) {
		l = p_l;
		r = p_r;
	}
	_ALWAYS_INLINE_ AudioFrame(const AudioFrame &p_frame) {
		l = p_frame.l;
		r = p_frame.r;
	}

	_ALWAYS_INLINE_ void operator=(const AudioFrame &p_frame) {
		l = p_frame.l;
		r = p_frame.r;
	}

	_ALWAYS_INLINE_ operator Vector2() const {
		return Vector2(l, r);
	}

	_ALWAYS_INLINE_ AudioFrame(const Vector2 &p_v2) {
		l = p_v2.x;
		r = p_v2.y;
	}
	_ALWAYS_INLINE_ AudioFrame() {}
};

_ALWAYS_INLINE_ AudioFrame operator*(float p_scalar, const AudioFrame &p_frame) {
	return AudioFrame(p_frame.l * p_scalar, p_frame.r * p_scalar);
}

_ALWAYS_INLINE_ AudioFrame operator*(int32_t p_scalar, const AudioFrame &p_frame) {
	return AudioFrame(p_frame.l * p_scalar, p_frame.r * p_scalar);
}

_ALWAYS_INLINE_ AudioFrame operator*(int64_t p_scalar, const AudioFrame &p_frame) {
	return AudioFrame(p_frame.l * p_scalar, p_frame.r * p_scalar);
}

#endif // AUDIO_FRAME_H
