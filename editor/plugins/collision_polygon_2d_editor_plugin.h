// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef COLLISION_POLYGON_2D_EDITOR_PLUGIN_H
#define COLLISION_POLYGON_2D_EDITOR_PLUGIN_H

#include "editor/plugins/abstract_polygon_2d_editor.h"
#include "scene/2d/collision_polygon_2d.h"

class CollisionPolygon2DEditor : public AbstractPolygon2DEditor {
	GDCLASS(CollisionPolygon2DEditor, AbstractPolygon2DEditor);

	CollisionPolygon2D *node = nullptr;

protected:
	virtual Node2D *_get_node() const override;
	virtual void _set_node(Node *p_polygon) override;

public:
	CollisionPolygon2DEditor();
};

class CollisionPolygon2DEditorPlugin : public AbstractPolygon2DEditorPlugin {
	GDCLASS(CollisionPolygon2DEditorPlugin, AbstractPolygon2DEditorPlugin);

public:
	CollisionPolygon2DEditorPlugin();
};

#endif // COLLISION_POLYGON_2D_EDITOR_PLUGIN_H
