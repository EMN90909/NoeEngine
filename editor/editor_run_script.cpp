// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "editor_run_script.h"

#include "editor/editor_node.h"

void EditorScript::add_root_node(Node *p_node) {
	if (!editor) {
		EditorNode::add_io_error("EditorScript::add_root_node: " + TTR("Write your logic in the _run() method."));
		return;
	}

	if (editor->get_edited_scene()) {
		EditorNode::add_io_error("EditorScript::add_root_node: " + TTR("There is an edited scene already."));
		return;
	}

	//editor->set_edited_scene(p_node);
}

EditorInterface *EditorScript::get_editor_interface() {
	return EditorInterface::get_singleton();
}

Node *EditorScript::get_scene() {
	if (!editor) {
		EditorNode::add_io_error("EditorScript::get_scene: " + TTR("Write your logic in the _run() method."));
		return nullptr;
	}

	return editor->get_edited_scene();
}

void EditorScript::_run() {
	if (!GDVIRTUAL_CALL(_run)) {
		EditorNode::add_io_error(TTR("Couldn't run editor script, did you forget to override the '_run' method?"));
	}
}

void EditorScript::set_editor(EditorNode *p_editor) {
	editor = p_editor;
}

void EditorScript::_bind_methods() {
	ClassDB::bind_method(D_METHOD("add_root_node", "node"), &EditorScript::add_root_node);
	ClassDB::bind_method(D_METHOD("get_scene"), &EditorScript::get_scene);
	ClassDB::bind_method(D_METHOD("get_editor_interface"), &EditorScript::get_editor_interface);
	GDVIRTUAL_BIND(_run);
}

EditorScript::EditorScript() {
	editor = nullptr;
}
