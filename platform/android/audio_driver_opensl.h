// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef AUDIO_DRIVER_OPENSL_H
#define AUDIO_DRIVER_OPENSL_H

#include "core/os/mutex.h"
#include "servers/audio_server.h"

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

class AudioDriverOpenSL : public AudioDriver {
	bool active = false;
	Mutex mutex;

	enum {
		BUFFER_COUNT = 2
	};

	bool pause = false;

	uint32_t buffer_size = 0;
	int16_t *buffers[BUFFER_COUNT] = {};
	int32_t *mixdown_buffer = nullptr;
	int last_free = 0;

	Vector<int16_t> rec_buffer;

	SLPlayItf playItf;
	SLRecordItf recordItf;
	SLObjectItf sl;
	SLEngineItf EngineItf;
	SLObjectItf OutputMix;
	SLObjectItf player;
	SLObjectItf recorder;
	SLAndroidSimpleBufferQueueItf bufferQueueItf;
	SLAndroidSimpleBufferQueueItf recordBufferQueueItf;
	SLDataSource audioSource;
	SLDataFormat_PCM pcm;
	SLDataSink audioSink;
	SLDataLocator_OutputMix locator_outputmix;

	static AudioDriverOpenSL *s_ad;

	void _buffer_callback(
			SLAndroidSimpleBufferQueueItf queueItf);

	static void _buffer_callbacks(
			SLAndroidSimpleBufferQueueItf queueItf,
			void *pContext);

	void _record_buffer_callback(
			SLAndroidSimpleBufferQueueItf queueItf);

	static void _record_buffer_callbacks(
			SLAndroidSimpleBufferQueueItf queueItf,
			void *pContext);

	Error init_input_device();

public:
	virtual const char *get_name() const override {
		return "Android";
	}

	virtual Error init() override;
	virtual void start() override;
	virtual int get_mix_rate() const override;
	virtual SpeakerMode get_speaker_mode() const override;

	virtual void lock() override;
	virtual void unlock() override;
	virtual void finish() override;

	virtual Error input_start() override;
	virtual Error input_stop() override;

	void set_pause(bool p_pause);

	AudioDriverOpenSL();
};

#endif // AUDIO_DRIVER_OPENSL_H
