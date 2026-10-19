// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "camera_3d_editor_plugin.h"

#include "editor/editor_node.h"
#include "node_3d_editor_plugin.h"

void Camera3DEditor::_node_removed(Node *p_node) {
	if (p_node == node) {
		node = nullptr;
		Node3DEditor::get_singleton()->set_custom_camera(nullptr);
		hide();
	}
}

void Camera3DEditor::_pressed() {
	Node *sn = (node && preview->is_pressed()) ? node : nullptr;
	Node3DEditor::get_singleton()->set_custom_camera(sn);
}

void Camera3DEditor::_bind_methods() {
}

void Camera3DEditor::edit(Node *p_camera) {
	node = p_camera;

	if (!node) {
		preview->set_pressed(false);
		Node3DEditor::get_singleton()->set_custom_camera(nullptr);
	} else {
		if (preview->is_pressed()) {
			Node3DEditor::get_singleton()->set_custom_camera(p_camera);
		} else {
			Node3DEditor::get_singleton()->set_custom_camera(nullptr);
		}
	}
}

Camera3DEditor::Camera3DEditor() {
	preview = memnew(Button);
	add_child(preview);

	preview->set_text(TTR("Preview"));
	preview->set_toggle_mode(true);
	preview->set_anchor(SIDE_LEFT, Control::ANCHOR_END);
	preview->set_anchor(SIDE_RIGHT, Control::ANCHOR_END);
	preview->set_offset(SIDE_LEFT, -60);
	preview->set_offset(SIDE_RIGHT, 0);
	preview->set_offset(SIDE_TOP, 0);
	preview->set_offset(SIDE_BOTTOM, 10);
	preview->connect("pressed", callable_mp(this, &Camera3DEditor::_pressed));
}

void Camera3DEditorPlugin::edit(Object *p_object) {
	Node3DEditor::get_singleton()->set_can_preview(Object::cast_to<Camera3D>(p_object));
	//camera_editor->edit(Object::cast_to<Node>(p_object));
}

bool Camera3DEditorPlugin::handles(Object *p_object) const {
	return p_object->is_class("Camera3D");
}

void Camera3DEditorPlugin::make_visible(bool p_visible) {
	if (p_visible) {
		//Node3DEditor::get_singleton()->set_can_preview(Object::cast_to<Camera3D>(p_object));
	} else {
		Node3DEditor::get_singleton()->set_can_preview(nullptr);
	}
}

Camera3DEditorPlugin::Camera3DEditorPlugin() {
	/*	camera_editor = memnew( CameraEditor );
	EditorNode::get_singleton()->get_main_screen_control()->add_child(camera_editor);

	camera_editor->set_anchor(SIDE_LEFT,Control::ANCHOR_END);
	camera_editor->set_anchor(SIDE_RIGHT,Control::ANCHOR_END);
	camera_editor->set_offset(SIDE_LEFT,60);
	camera_editor->set_offset(SIDE_RIGHT,0);
	camera_editor->set_offset(SIDE_TOP,0);
	camera_editor->set_offset(SIDE_BOTTOM,10);


	camera_editor->hide();
*/
}

Camera3DEditorPlugin::~Camera3DEditorPlugin() {
}
