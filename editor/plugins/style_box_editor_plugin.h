// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef STYLE_BOX_EDITOR_PLUGIN_H
#define STYLE_BOX_EDITOR_PLUGIN_H

#include "editor/editor_inspector.h"
#include "editor/editor_plugin.h"
#include "scene/gui/option_button.h"
#include "scene/gui/texture_rect.h"
#include "scene/resources/style_box.h"

class TextureButton;

class StyleBoxPreview : public VBoxContainer {
	GDCLASS(StyleBoxPreview, VBoxContainer);

	TextureRect *checkerboard = nullptr;
	TextureButton *grid_preview = nullptr;
	Control *preview = nullptr;
	Ref<StyleBox> stylebox;

	void _sb_changed();
	void _redraw();
	void _notification(int p_what);
	static bool grid_preview_enabled;
	void _grid_preview_toggled(bool p_active);

protected:
	static void _bind_methods();

public:
	void edit(const Ref<StyleBox> &p_stylebox);

	StyleBoxPreview();
};

class EditorInspectorPluginStyleBox : public EditorInspectorPlugin {
	GDCLASS(EditorInspectorPluginStyleBox, EditorInspectorPlugin);

public:
	virtual bool can_handle(Object *p_object) override;
	virtual void parse_begin(Object *p_object) override;
};

class StyleBoxEditorPlugin : public EditorPlugin {
	GDCLASS(StyleBoxEditorPlugin, EditorPlugin);

public:
	virtual String get_name() const override { return "StyleBox"; }

	StyleBoxEditorPlugin();
};

#endif // STYLE_BOX_EDITOR_PLUGIN_H
