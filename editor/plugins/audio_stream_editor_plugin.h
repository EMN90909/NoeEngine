// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef AUDIO_STREAM_EDITOR_PLUGIN_H
#define AUDIO_STREAM_EDITOR_PLUGIN_H

#include "editor/editor_inspector.h"
#include "editor/editor_plugin.h"
#include "scene/audio/audio_stream_player.h"
#include "scene/gui/button.h"
#include "scene/gui/color_rect.h"
#include "scene/gui/label.h"

class AudioStreamEditor : public ColorRect {
	GDCLASS(AudioStreamEditor, ColorRect);

	Ref<AudioStream> stream;

	AudioStreamPlayer *_player = nullptr;
	ColorRect *_preview = nullptr;
	Control *_indicator = nullptr;
	Label *_current_label = nullptr;
	Label *_duration_label = nullptr;

	Button *_play_button = nullptr;
	Button *_stop_button = nullptr;

	float _current = 0;
	bool _dragging = false;
	bool _pausing = false;

protected:
	void _notification(int p_what);
	void _preview_changed(ObjectID p_which);
	void _play();
	void _stop();
	void _on_finished();
	void _draw_preview();
	void _draw_indicator();
	void _on_input_indicator(Ref<InputEvent> p_event);
	void _seek_to(real_t p_x);
	void _stream_changed();

public:
	void set_stream(const Ref<AudioStream> &p_stream);

	AudioStreamEditor();
};

class EditorInspectorPluginAudioStream : public EditorInspectorPlugin {
	GDCLASS(EditorInspectorPluginAudioStream, EditorInspectorPlugin);
	AudioStreamEditor *editor = nullptr;

public:
	virtual bool can_handle(Object *p_object) override;
	virtual void parse_begin(Object *p_object) override;
};

class AudioStreamEditorPlugin : public EditorPlugin {
	GDCLASS(AudioStreamEditorPlugin, EditorPlugin);

public:
	AudioStreamEditorPlugin();
};

#endif // AUDIO_STREAM_EDITOR_PLUGIN_H
