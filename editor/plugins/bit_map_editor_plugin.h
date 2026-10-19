// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef BIT_MAP_EDITOR_PLUGIN_H
#define BIT_MAP_EDITOR_PLUGIN_H

#include "editor/editor_inspector.h"
#include "editor/editor_plugin.h"
#include "scene/resources/bit_map.h"

class TextureRect;

class BitMapEditor : public VBoxContainer {
	GDCLASS(BitMapEditor, VBoxContainer);

	TextureRect *texture_rect = nullptr;
	Label *size_label = nullptr;

public:
	void setup(const Ref<BitMap> &p_bitmap);

	BitMapEditor();
};

class EditorInspectorPluginBitMap : public EditorInspectorPlugin {
	GDCLASS(EditorInspectorPluginBitMap, EditorInspectorPlugin);

public:
	virtual bool can_handle(Object *p_object) override;
	virtual void parse_begin(Object *p_object) override;
};

class BitMapEditorPlugin : public EditorPlugin {
	GDCLASS(BitMapEditorPlugin, EditorPlugin);

public:
	BitMapEditorPlugin();
};

#endif // BIT_MAP_EDITOR_PLUGIN_H
