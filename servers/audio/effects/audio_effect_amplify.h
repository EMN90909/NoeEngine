// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef AUDIO_EFFECT_AMPLIFY_H
#define AUDIO_EFFECT_AMPLIFY_H

#include "servers/audio/audio_effect.h"

class AudioEffectAmplify;

class AudioEffectAmplifyInstance : public AudioEffectInstance {
	GDCLASS(AudioEffectAmplifyInstance, AudioEffectInstance);
	friend class AudioEffectAmplify;
	Ref<AudioEffectAmplify> base;

	float mix_volume_db;

public:
	virtual void process(const AudioFrame *p_src_frames, AudioFrame *p_dst_frames, int p_frame_count) override;
};

class AudioEffectAmplify : public AudioEffect {
	GDCLASS(AudioEffectAmplify, AudioEffect);

	friend class AudioEffectAmplifyInstance;
	float volume_db;

protected:
	static void _bind_methods();

public:
	Ref<AudioEffectInstance> instantiate() override;
	void set_volume_db(float p_volume);
	float get_volume_db() const;

	AudioEffectAmplify();
};

#endif // AUDIO_EFFECT_AMPLIFY_H
