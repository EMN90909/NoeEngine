// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef TTS_IOS_H
#define TTS_IOS_H

#if __has_include(<AVFAudio/AVSpeechSynthesis.h>)
#import <AVFAudio/AVSpeechSynthesis.h>
#else
#import <AVFoundation/AVFoundation.h>
#endif

#include "core/string/ustring.h"
#include "core/templates/hash_map.h"
#include "core/templates/list.h"
#include "core/variant/array.h"
#include "servers/display_server.h"

@interface TTS_IOS : NSObject <AVSpeechSynthesizerDelegate> {
	bool speaking;
	HashMap<id, int> ids;

	AVSpeechSynthesizer *av_synth;
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

#endif // TTS_IOS_H
