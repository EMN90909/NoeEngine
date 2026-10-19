// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef TTS_WINDOWS_H
#define TTS_WINDOWS_H

#include "core/string/ustring.h"
#include "core/templates/hash_map.h"
#include "core/templates/list.h"
#include "core/variant/array.h"
#include "servers/display_server.h"

#include <objbase.h>
#include <sapi.h>
#include <wchar.h>
#include <winnls.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class TTS_Windows {
	List<DisplayServer::TTSUtterance> queue;
	ISpVoice *synth = nullptr;
	bool paused = false;
	struct UTData {
		Char16String string;
		int offset;
		int id;
	};
	HashMap<uint32_t, UTData> ids;

	static void __stdcall speech_event_callback(WPARAM wParam, LPARAM lParam);
	void _update_tts();

	static TTS_Windows *singleton;

public:
	static TTS_Windows *get_singleton();

	bool is_speaking() const;
	bool is_paused() const;
	Array get_voices() const;

	void speak(const String &p_text, const String &p_voice, int p_volume = 50, float p_pitch = 1.f, float p_rate = 1.f, int p_utterance_id = 0, bool p_interrupt = false);
	void pause();
	void resume();
	void stop();

	TTS_Windows();
	~TTS_Windows();
};

#endif // TTS_WINDOWS_H
