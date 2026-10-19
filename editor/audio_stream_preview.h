// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef AUDIO_STREAM_PREVIEW_H
#define AUDIO_STREAM_PREVIEW_H

#include "core/os/thread.h"
#include "core/templates/safe_refcount.h"
#include "scene/main/node.h"
#include "servers/audio/audio_stream.h"

class AudioStreamPreview : public RefCounted {
	GDCLASS(AudioStreamPreview, RefCounted);
	friend class AudioStream;
	Vector<uint8_t> preview;
	float length;

	friend class AudioStreamPreviewGenerator;
	uint64_t version = 1;

public:
	uint64_t get_version() const { return version; }
	float get_length() const;
	float get_max(float p_time, float p_time_next) const;
	float get_min(float p_time, float p_time_next) const;

	AudioStreamPreview();
};

class AudioStreamPreviewGenerator : public Node {
	GDCLASS(AudioStreamPreviewGenerator, Node);

	static AudioStreamPreviewGenerator *singleton;

	struct Preview {
		Ref<AudioStreamPreview> preview;
		Ref<AudioStream> base_stream;
		Ref<AudioStreamPlayback> playback;
		SafeFlag generating;
		ObjectID id;
		Thread *thread = nullptr;

		// Needed for the bookkeeping of the Map
		void operator=(const Preview &p_rhs) {
			preview = p_rhs.preview;
			base_stream = p_rhs.base_stream;
			playback = p_rhs.playback;
			generating.set_to(generating.is_set());
			id = p_rhs.id;
			thread = p_rhs.thread;
		}
		Preview(const Preview &p_rhs) {
			preview = p_rhs.preview;
			base_stream = p_rhs.base_stream;
			playback = p_rhs.playback;
			generating.set_to(generating.is_set());
			id = p_rhs.id;
			thread = p_rhs.thread;
		}
		Preview() {}
	};

	HashMap<ObjectID, Preview> previews;

	static void _preview_thread(void *p_preview);

	void _update_emit(ObjectID p_id);

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	static AudioStreamPreviewGenerator *get_singleton() { return singleton; }

	Ref<AudioStreamPreview> generate_preview(const Ref<AudioStream> &p_stream);

	AudioStreamPreviewGenerator();
};

#endif // AUDIO_STREAM_PREVIEW_H
