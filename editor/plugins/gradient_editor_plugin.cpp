// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "gradient_editor_plugin.h"

#include "canvas_item_editor_plugin.h"
#include "editor/editor_node.h"
#include "editor/editor_scale.h"
#include "editor/editor_settings.h"
#include "editor/editor_undo_redo_manager.h"
#include "node_3d_editor_plugin.h"

void GradientReverseButton::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_DRAW: {
			Ref<Texture2D> icon = get_theme_icon(SNAME("ReverseGradient"), SNAME("EditorIcons"));
			if (is_pressed()) {
				draw_texture_rect(icon, Rect2(margin, margin, icon->get_width(), icon->get_height()), false, get_theme_color(SNAME("icon_pressed_color"), SNAME("Button")));
			} else {
				draw_texture_rect(icon, Rect2(margin, margin, icon->get_width(), icon->get_height()));
			}
		} break;
	}
}

Size2 GradientReverseButton::get_minimum_size() const {
	return (get_theme_icon(SNAME("ReverseGradient"), SNAME("EditorIcons"))->get_size() + Size2(margin * 2, margin * 2));
}

///////////////////////

bool EditorInspectorPluginGradient::can_handle(Object *p_object) {
	return Object::cast_to<Gradient>(p_object) != nullptr;
}

void EditorInspectorPluginGradient::parse_begin(Object *p_object) {
	Gradient *gradient = Object::cast_to<Gradient>(p_object);
	Ref<Gradient> g(gradient);

	editor = memnew(GradientEditor);
	editor->set_gradient(g);
	add_custom_control(editor);

	int picker_shape = EDITOR_GET("interface/inspector/default_color_picker_shape");
	editor->get_picker()->set_picker_shape((ColorPicker::PickerShapeType)picker_shape);

	reverse_btn = memnew(GradientReverseButton);

	gradient_tools_hbox = memnew(HBoxContainer);
	gradient_tools_hbox->add_child(reverse_btn);

	add_custom_control(gradient_tools_hbox);

	reverse_btn->connect("pressed", callable_mp(this, &EditorInspectorPluginGradient::_reverse_button_pressed));
	reverse_btn->set_tooltip_text(TTR("Reverse/mirror gradient."));
}

void EditorInspectorPluginGradient::_reverse_button_pressed() {
	editor->reverse_gradient();
}

GradientEditorPlugin::GradientEditorPlugin() {
	Ref<EditorInspectorPluginGradient> plugin;
	plugin.instantiate();
	add_inspector_plugin(plugin);
}
