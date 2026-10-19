// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef AUDIO_STREAM_GENERATOR_H
#define AUDIO_STREAM_GENERATOR_H

#include "core/templates/ring_buffer.h"
#include "servers/audio/audio_stream.h"

class AudioStreamGenerator : public AudioStream {
	GDCLASS(AudioStreamGenerator, AudioStream);

	float mix_rate;
	float buffer_len;

protected:
	static void _bind_methods();

public:
	void set_mix_rate(float p_mix_rate);
	float get_mix_rate() const;

	void set_buffer_length(float p_seconds);
	float get_buffer_length() const;

	virtual Ref<AudioStreamPlayback> instantiate_playback() override;
	virtual String get_stream_name() const override;

	virtual double get_length() const override;
	virtual bool is_monophonic() const override;
	AudioStreamGenerator();
};

class AudioStreamGeneratorPlayback : public AudioStreamPlaybackResampled {
	GDCLASS(AudioStreamGeneratorPlayback, AudioStreamPlaybackResampled);
	friend class AudioStreamGenerator;
	RingBuffer<AudioFrame> buffer;
	int skips;
	bool active;
	float mixed;
	AudioStreamGenerator *generator = nullptr;

protected:
	virtual int _mix_internal(AudioFrame *p_buffer, int p_frames) override;
	virtual float get_stream_sampling_rate() override;

	static void _bind_methods();

public:
	virtual void start(double p_from_pos = 0.0) override;
	virtual void stop() override;
	virtual bool is_playing() const override;

	virtual int get_loop_count() const override; //times it looped

	virtual double get_playback_position() const override;
	virtual void seek(double p_time) override;

	bool push_frame(const Vector2 &p_frame);
	bool can_push_buffer(int p_frames) const;
	bool push_buffer(const PackedVector2Array &p_frames);
	int get_frames_available() const;
	int get_skips() const;

	virtual void tag_used_streams() override;

	void clear_buffer();

	AudioStreamGeneratorPlayback();
};

#endif // AUDIO_STREAM_GENERATOR_H
