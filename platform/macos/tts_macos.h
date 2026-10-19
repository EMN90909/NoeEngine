// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef TTS_MACOS_H
#define TTS_MACOS_H

#include "core/string/ustring.h"
#include "core/templates/hash_map.h"
#include "core/templates/list.h"
#include "core/variant/array.h"
#include "servers/display_server.h"

#import <AppKit/AppKit.h>

#if __has_include(<AVFAudio/AVSpeechSynthesis.h>)
#import <AVFAudio/AVSpeechSynthesis.h>
#else
#import <AVFoundation/AVFoundation.h>
#endif

@interface TTS_MacOS : NSObject <AVSpeechSynthesizerDelegate> {
	// AVSpeechSynthesizer
	bool speaking;
	HashMap<id, int> ids;

	// NSSpeechSynthesizer
	bool paused;
	bool have_utterance;
	int last_utterance;

	id synth; // NSSpeechSynthesizer or AVSpeechSynthesizer
	List<DisplayServer::TTSUtterance> queue;
}

- (void)pauseSpeaking;
- (void)resumeSpeaking;
- (void)stopSpeaking;
- (bool)isSpeaking;
- (bool)isPaused;
- (void)speak:(const String &)text voice:(const String &)voice volume:(int)volume pitch:(float)pitch rate:(float)rate utterance_id:(int)utterance_id interrupt:(bool)interrupt;
- (Array)getVoices;
@end

#endif // TTS_MACOS_H
