// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef VIDEO_STREAM_H
#define VIDEO_STREAM_H

#include "core/io/file_access.h"
#include "scene/resources/texture.h"

class VideoStreamPlayback : public Resource {
	GDCLASS(VideoStreamPlayback, Resource);

public:
	typedef int (*AudioMixCallback)(void *p_udata, const float *p_data, int p_frames);

protected:
	AudioMixCallback mix_callback = nullptr;
	void *mix_udata = nullptr;
	mutable int _channel_count = 0; // Used only to assist with bounds checking in mix_audio.

	static void _bind_methods();
	GDVIRTUAL0(_stop);
	GDVIRTUAL0(_play);
	GDVIRTUAL0RC(bool, _is_playing);
	GDVIRTUAL1(_set_paused, bool);
	GDVIRTUAL0RC(bool, _is_paused);
	GDVIRTUAL0RC(double, _get_length);
	GDVIRTUAL0RC(double, _get_playback_position);
	GDVIRTUAL1(_seek, double);
	GDVIRTUAL1(_set_audio_track, int);
	GDVIRTUAL0RC(Ref<Texture2D>, _get_texture);
	GDVIRTUAL1(_update, double);
	GDVIRTUAL0RC(int, _get_channels);
	GDVIRTUAL0RC(int, _get_mix_rate);

	int mix_audio(int num_frames, PackedFloat32Array buffer = {}, int offset = 0);

public:
	VideoStreamPlayback();
	virtual ~VideoStreamPlayback();

	virtual void stop();
	virtual void play();

	virtual bool is_playing() const;

	virtual void set_paused(bool p_paused);
	virtual bool is_paused() const;

	virtual double get_length() const;

	virtual double get_playback_position() const;
	virtual void seek(double p_time);

	virtual void set_audio_track(int p_idx);

	virtual Ref<Texture2D> get_texture() const;
	virtual void update(double p_delta);

	virtual void set_mix_callback(AudioMixCallback p_callback, void *p_userdata);
	virtual int get_channels() const;
	virtual int get_mix_rate() const;
};

class VideoStream : public Resource {
	GDCLASS(VideoStream, Resource);
	OBJ_SAVE_TYPE(VideoStream);

protected:
	static void
	_bind_methods();

	GDVIRTUAL0R(Ref<VideoStreamPlayback>, _instantiate_playback);

	String file;
	int audio_track = 0;

public:
	void set_file(const String &p_file);
	String get_file();

	virtual void set_audio_track(int p_track);
	virtual Ref<VideoStreamPlayback> instantiate_playback();

	VideoStream();
	~VideoStream();
};

#endif // VIDEO_STREAM_H
