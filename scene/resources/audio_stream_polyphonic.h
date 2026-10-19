// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef AUDIO_STREAM_POLYPHONIC_H
#define AUDIO_STREAM_POLYPHONIC_H

#include "core/templates/local_vector.h"
#include "servers/audio/audio_stream.h"

class AudioStreamPolyphonic : public AudioStream {
	GDCLASS(AudioStreamPolyphonic, AudioStream)
	int polyphony = 32;

	static void _bind_methods();

public:
	virtual Ref<AudioStreamPlayback> instantiate_playback() override;
	virtual String get_stream_name() const override;
	virtual bool is_monophonic() const override;

	void set_polyphony(int p_voices);
	int get_polyphony() const;

	AudioStreamPolyphonic();
};

class AudioStreamPlaybackPolyphonic : public AudioStreamPlayback {
	GDCLASS(AudioStreamPlaybackPolyphonic, AudioStreamPlayback)

	enum {
		INTERNAL_BUFFER_LEN = 128,
		ID_MASK = 0xFFFFFFFF,
		INDEX_SHIFT = 32
	};
	struct Stream {
		SafeFlag active;
		SafeFlag pending_play;
		SafeFlag finish_request;
		float play_offset = 0;
		float pitch_scale = 1.0;
		Ref<AudioStream> stream;
		Ref<AudioStreamPlayback> stream_playback;
		float prev_volume_db = 0;
		float volume_db = 0;
		uint32_t id = 0;

		Stream() :
				active(false), pending_play(false), finish_request(false) {}
	};

	LocalVector<Stream> streams;
	AudioFrame internal_buffer[INTERNAL_BUFFER_LEN];

	bool active = false;
	uint32_t id_counter = 1;

	_FORCE_INLINE_ Stream *_find_stream(int64_t p_id);

	friend class AudioStreamPolyphonic;

protected:
	static void _bind_methods();

public:
	typedef int64_t ID;
	enum {
		INVALID_ID = -1
	};

	virtual void start(double p_from_pos = 0.0) override;
	virtual void stop() override;
	virtual bool is_playing() const override;

	virtual int get_loop_count() const override; //times it looped

	virtual double get_playback_position() const override;
	virtual void seek(double p_time) override;

	virtual void tag_used_streams() override;

	virtual int mix(AudioFrame *p_buffer, float p_rate_scale, int p_frames) override;

	ID play_stream(const Ref<AudioStream> &p_stream, float p_from_offset = 0, float p_volume_db = 0, float p_pitch_scale = 1.0);
	void set_stream_volume(ID p_stream_id, float p_volume_db);
	void set_stream_pitch_scale(ID p_stream_id, float p_pitch_scale);
	bool is_stream_playing(ID p_stream_id) const;
	void stop_stream(ID p_stream_id);

	AudioStreamPlaybackPolyphonic();
};

#endif // AUDIO_STREAM_POLYPHONIC_H
