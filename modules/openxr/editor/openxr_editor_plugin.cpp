// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "openxr_editor_plugin.h"

#include "../action_map/openxr_action_map.h"
#include "editor/editor_node.h"

void OpenXREditorPlugin::edit(Object *p_node) {
	if (Object::cast_to<OpenXRActionMap>(p_node)) {
		String path = Object::cast_to<OpenXRActionMap>(p_node)->get_path();
		if (path.is_resource_file()) {
			action_map_editor->open_action_map(path);
		}
	}
}

bool OpenXREditorPlugin::handles(Object *p_node) const {
	return (Object::cast_to<OpenXRActionMap>(p_node) != nullptr);
}

void OpenXREditorPlugin::make_visible(bool p_visible) {
}

OpenXREditorPlugin::OpenXREditorPlugin() {
	action_map_editor = memnew(OpenXRActionMapEditor);
	EditorNode::get_singleton()->add_bottom_panel_item(TTR("OpenXR Action Map"), action_map_editor);
}

OpenXREditorPlugin::~OpenXREditorPlugin() {
}
