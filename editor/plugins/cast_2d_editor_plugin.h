// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef CAST_2D_EDITOR_PLUGIN_H
#define CAST_2D_EDITOR_PLUGIN_H

#include "editor/editor_plugin.h"
#include "scene/2d/node_2d.h"

class CanvasItemEditor;

class Cast2DEditor : public Control {
	GDCLASS(Cast2DEditor, Control);

	CanvasItemEditor *canvas_item_editor = nullptr;
	Node2D *node = nullptr;

	bool pressed = false;
	Point2 original_target_position;

protected:
	void _notification(int p_what);
	void _node_removed(Node *p_node);

public:
	bool forward_canvas_gui_input(const Ref<InputEvent> &p_event);
	void forward_canvas_draw_over_viewport(Control *p_overlay);
	void edit(Node2D *p_node);
};

class Cast2DEditorPlugin : public EditorPlugin {
	GDCLASS(Cast2DEditorPlugin, EditorPlugin);

	Cast2DEditor *cast_2d_editor = nullptr;

public:
	virtual bool forward_canvas_gui_input(const Ref<InputEvent> &p_event) override { return cast_2d_editor->forward_canvas_gui_input(p_event); }
	virtual void forward_canvas_draw_over_viewport(Control *p_overlay) override { cast_2d_editor->forward_canvas_draw_over_viewport(p_overlay); }

	virtual String get_name() const override { return "Cast2D"; }
	bool has_main_screen() const override { return false; }
	virtual void edit(Object *p_object) override;
	virtual bool handles(Object *p_object) const override;
	virtual void make_visible(bool visible) override;

	Cast2DEditorPlugin();
};

#endif // CAST_2D_EDITOR_PLUGIN_H
