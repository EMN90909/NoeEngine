// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef AUDIO_EFFECT_H
#define AUDIO_EFFECT_H

#include "core/io/resource.h"
#include "core/math/audio_frame.h"
#include "core/object/gdvirtual.gen.inc"
#include "core/object/script_language.h"
#include "core/variant/native_ptr.h"

class AudioEffectInstance : public RefCounted {
	GDCLASS(AudioEffectInstance, RefCounted);

protected:
	GDVIRTUAL3(_process, GDExtensionConstPtr<AudioFrame>, GDExtensionPtr<AudioFrame>, int)
	GDVIRTUAL0RC(bool, _process_silence)
	static void _bind_methods();

public:
	virtual void process(const AudioFrame *p_src_frames, AudioFrame *p_dst_frames, int p_frame_count);
	virtual bool process_silence() const;
};

class AudioEffect : public Resource {
	GDCLASS(AudioEffect, Resource);

protected:
	GDVIRTUAL0R(Ref<AudioEffectInstance>, _instantiate)
	static void _bind_methods();

public:
	virtual Ref<AudioEffectInstance> instantiate();
	AudioEffect();
};

#endif // AUDIO_EFFECT_H
