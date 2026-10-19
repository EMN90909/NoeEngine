// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "audio_effect_amplify.h"

void AudioEffectAmplifyInstance::process(const AudioFrame *p_src_frames, AudioFrame *p_dst_frames, int p_frame_count) {
	//multiply volume interpolating to avoid clicks if this changes
	float volume_db = base->volume_db;
	float vol = Math::db_to_linear(mix_volume_db);
	float vol_inc = (Math::db_to_linear(volume_db) - vol) / float(p_frame_count);

	for (int i = 0; i < p_frame_count; i++) {
		p_dst_frames[i] = p_src_frames[i] * vol;
		vol += vol_inc;
	}
	//set volume for next mix
	mix_volume_db = volume_db;
}

Ref<AudioEffectInstance> AudioEffectAmplify::instantiate() {
	Ref<AudioEffectAmplifyInstance> ins;
	ins.instantiate();
	ins->base = Ref<AudioEffectAmplify>(this);
	ins->mix_volume_db = volume_db;
	return ins;
}

void AudioEffectAmplify::set_volume_db(float p_volume) {
	volume_db = p_volume;
}

float AudioEffectAmplify::get_volume_db() const {
	return volume_db;
}

void AudioEffectAmplify::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_volume_db", "volume"), &AudioEffectAmplify::set_volume_db);
	ClassDB::bind_method(D_METHOD("get_volume_db"), &AudioEffectAmplify::get_volume_db);

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "volume_db", PROPERTY_HINT_RANGE, "-80,24,0.01,suffix:dB"), "set_volume_db", "get_volume_db");
}

AudioEffectAmplify::AudioEffectAmplify() {
	volume_db = 0;
}
