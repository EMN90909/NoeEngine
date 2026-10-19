// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "audio_effect.h"

void AudioEffectInstance::process(const AudioFrame *p_src_frames, AudioFrame *p_dst_frames, int p_frame_count) {
	GDVIRTUAL_REQUIRED_CALL(_process, p_src_frames, p_dst_frames, p_frame_count);
}
bool AudioEffectInstance::process_silence() const {
	bool ret = false;
	GDVIRTUAL_CALL(_process_silence, ret);
	return ret;
}

void AudioEffectInstance::_bind_methods() {
	GDVIRTUAL_BIND(_process, "src_buffer", "dst_buffer", "frame_count");
	GDVIRTUAL_BIND(_process_silence);
}

////

Ref<AudioEffectInstance> AudioEffect::instantiate() {
	Ref<AudioEffectInstance> ret;
	GDVIRTUAL_REQUIRED_CALL(_instantiate, ret);
	return ret;
}
void AudioEffect::_bind_methods() {
	GDVIRTUAL_BIND(_instantiate);
}

AudioEffect::AudioEffect() {
}
