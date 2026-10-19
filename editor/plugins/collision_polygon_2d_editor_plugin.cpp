// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "collision_polygon_2d_editor_plugin.h"

Node2D *CollisionPolygon2DEditor::_get_node() const {
	return node;
}

void CollisionPolygon2DEditor::_set_node(Node *p_polygon) {
	node = Object::cast_to<CollisionPolygon2D>(p_polygon);
}

CollisionPolygon2DEditor::CollisionPolygon2DEditor() {}

CollisionPolygon2DEditorPlugin::CollisionPolygon2DEditorPlugin() :
		AbstractPolygon2DEditorPlugin(memnew(CollisionPolygon2DEditor), "CollisionPolygon2D") {
}
