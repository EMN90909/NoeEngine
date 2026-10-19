// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef AUDIO_STREAM_RANDOMIZER_EDITOR_PLUGIN_H
#define AUDIO_STREAM_RANDOMIZER_EDITOR_PLUGIN_H

#include "editor/editor_plugin.h"
#include "servers/audio/audio_stream.h"

class AudioStreamRandomizerEditorPlugin : public EditorPlugin {
	GDCLASS(AudioStreamRandomizerEditorPlugin, EditorPlugin);

private:
	void _move_stream_array_element(Object *p_undo_redo, Object *p_edited, String p_array_prefix, int p_from_index, int p_to_pos);

public:
	virtual String get_name() const override { return "AudioStreamRandomizer"; }
	bool has_main_screen() const override { return false; }
	virtual void edit(Object *p_object) override;
	virtual bool handles(Object *p_object) const override;
	virtual void make_visible(bool p_visible) override;

	AudioStreamRandomizerEditorPlugin();
	~AudioStreamRandomizerEditorPlugin();
};

#endif // AUDIO_STREAM_RANDOMIZER_EDITOR_PLUGIN_H
