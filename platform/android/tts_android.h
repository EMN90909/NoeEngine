// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef TTS_ANDROID_H
#define TTS_ANDROID_H

#include "core/string/ustring.h"
#include "core/templates/hash_map.h"
#include "core/variant/array.h"
#include "servers/display_server.h"

#include <jni.h>

class TTS_Android {
	static jobject tts;
	static jclass cls;

	static jmethodID _is_speaking;
	static jmethodID _is_paused;
	static jmethodID _get_voices;
	static jmethodID _speak;
	static jmethodID _pause_speaking;
	static jmethodID _resume_speaking;
	static jmethodID _stop_speaking;

	static HashMap<int, Char16String> ids;

public:
	static void setup(jobject p_tts);
	static void _java_utterance_callback(int p_event, int p_id, int p_pos);

	static bool is_speaking();
	static bool is_paused();
	static Array get_voices();
	static void speak(const String &p_text, const String &p_voice, int p_volume, float p_pitch, float p_rate, int p_utterance_id, bool p_interrupt);
	static void pause();
	static void resume();
	static void stop();
};

#endif // TTS_ANDROID_H
