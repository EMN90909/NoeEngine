// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef TEXTURE_EDITOR_PLUGIN_H
#define TEXTURE_EDITOR_PLUGIN_H

#include "editor/editor_inspector.h"
#include "editor/editor_plugin.h"
#include "scene/gui/margin_container.h"
#include "scene/resources/texture.h"

class TextureRect;

class TexturePreview : public MarginContainer {
	GDCLASS(TexturePreview, MarginContainer);

private:
	TextureRect *texture_display = nullptr;

	TextureRect *checkerboard = nullptr;
	Label *metadata_label = nullptr;

	void _update_metadata_label_text();

protected:
	void _notification(int p_what);

public:
	TextureRect *get_texture_display();
	TexturePreview(Ref<Texture2D> p_texture, bool p_show_metadata);
};

class EditorInspectorPluginTexture : public EditorInspectorPlugin {
	GDCLASS(EditorInspectorPluginTexture, EditorInspectorPlugin);

public:
	virtual bool can_handle(Object *p_object) override;
	virtual void parse_begin(Object *p_object) override;
};

class TextureEditorPlugin : public EditorPlugin {
	GDCLASS(TextureEditorPlugin, EditorPlugin);

public:
	virtual String get_name() const override { return "Texture2D"; }

	TextureEditorPlugin();
};

#endif // TEXTURE_EDITOR_PLUGIN_H
