// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef GRADIENT_EDITOR_PLUGIN_H
#define GRADIENT_EDITOR_PLUGIN_H

#include "editor/editor_inspector.h"
#include "editor/editor_plugin.h"
#include "gradient_editor.h"

class GradientReverseButton : public BaseButton {
	GDCLASS(GradientReverseButton, BaseButton);

	int margin = 2;

	void _notification(int p_what);
	virtual Size2 get_minimum_size() const override;
};

class EditorInspectorPluginGradient : public EditorInspectorPlugin {
	GDCLASS(EditorInspectorPluginGradient, EditorInspectorPlugin);

	GradientEditor *editor = nullptr;
	HBoxContainer *gradient_tools_hbox = nullptr;
	GradientReverseButton *reverse_btn = nullptr;

	void _reverse_button_pressed();

public:
	virtual bool can_handle(Object *p_object) override;
	virtual void parse_begin(Object *p_object) override;
};

class GradientEditorPlugin : public EditorPlugin {
	GDCLASS(GradientEditorPlugin, EditorPlugin);

public:
	virtual String get_name() const override { return "Gradient"; }

	GradientEditorPlugin();
};

#endif // GRADIENT_EDITOR_PLUGIN_H
