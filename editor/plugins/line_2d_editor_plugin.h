// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef LINE_2D_EDITOR_PLUGIN_H
#define LINE_2D_EDITOR_PLUGIN_H

#include "editor/plugins/abstract_polygon_2d_editor.h"
#include "scene/2d/line_2d.h"

class Line2DEditor : public AbstractPolygon2DEditor {
	GDCLASS(Line2DEditor, AbstractPolygon2DEditor);

	Line2D *node = nullptr;

protected:
	virtual Node2D *_get_node() const override;
	virtual void _set_node(Node *p_line) override;

	virtual bool _is_line() const override;
	virtual Variant _get_polygon(int p_idx) const override;
	virtual void _set_polygon(int p_idx, const Variant &p_polygon) const override;
	virtual void _action_set_polygon(int p_idx, const Variant &p_previous, const Variant &p_polygon) override;

public:
	Line2DEditor();
};

class Line2DEditorPlugin : public AbstractPolygon2DEditorPlugin {
	GDCLASS(Line2DEditorPlugin, AbstractPolygon2DEditorPlugin);

public:
	Line2DEditorPlugin();
};

#endif // LINE_2D_EDITOR_PLUGIN_H
