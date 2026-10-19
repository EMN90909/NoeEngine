// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifdef TOOLS_ENABLED

#include "editor_scene_exporter_gltf_plugin.h"

#include "../gltf_document.h"

#include "editor/editor_file_dialog.h"
#include "editor/editor_file_system.h"
#include "editor/editor_node.h"

String SceneExporterGLTFPlugin::get_name() const {
	return "ConvertGLTF2";
}

bool SceneExporterGLTFPlugin::has_main_screen() const {
	return false;
}

SceneExporterGLTFPlugin::SceneExporterGLTFPlugin() {
	file_export_lib = memnew(EditorFileDialog);
	EditorNode::get_singleton()->get_gui_base()->add_child(file_export_lib);
	file_export_lib->connect("file_selected", callable_mp(this, &SceneExporterGLTFPlugin::_gltf2_dialog_action));
	file_export_lib->set_title(TTR("Export Library"));
	file_export_lib->set_file_mode(EditorFileDialog::FILE_MODE_SAVE_FILE);
	file_export_lib->set_access(EditorFileDialog::ACCESS_FILESYSTEM);
	file_export_lib->clear_filters();
	file_export_lib->add_filter("*.glb");
	file_export_lib->add_filter("*.gltf");
	file_export_lib->set_title(TTR("Export Scene to glTF 2.0 File"));

	PopupMenu *menu = get_export_as_menu();
	int idx = menu->get_item_count();
	menu->add_item(TTR("glTF 2.0 Scene..."));
	menu->set_item_metadata(idx, callable_mp(this, &SceneExporterGLTFPlugin::convert_scene_to_gltf2));
}

void SceneExporterGLTFPlugin::_gltf2_dialog_action(String p_file) {
	Node *root = EditorNode::get_singleton()->get_tree()->get_edited_scene_root();
	if (!root) {
		EditorNode::get_singleton()->show_accept(TTR("This operation can't be done without a scene."), TTR("OK"));
		return;
	}
	List<String> deps;
	Ref<GLTFDocument> doc;
	doc.instantiate();
	Ref<GLTFState> state;
	state.instantiate();
	int32_t flags = 0;
	flags |= EditorSceneFormatImporter::IMPORT_USE_NAMED_SKIN_BINDS;
	Error err = doc->append_from_scene(root, state, flags);
	if (err != OK) {
		ERR_PRINT(vformat("glTF2 save scene error %s.", itos(err)));
	}
	err = doc->write_to_filesystem(state, p_file);
	if (err != OK) {
		ERR_PRINT(vformat("glTF2 save scene error %s.", itos(err)));
	}
}

void SceneExporterGLTFPlugin::convert_scene_to_gltf2() {
	Node *root = EditorNode::get_singleton()->get_tree()->get_edited_scene_root();
	if (!root) {
		EditorNode::get_singleton()->show_accept(TTR("This operation can't be done without a scene."), TTR("OK"));
		return;
	}
	String filename = String(root->get_scene_file_path().get_file().get_basename());
	if (filename.is_empty()) {
		filename = root->get_name();
	}
	file_export_lib->set_current_file(filename + String(".gltf"));
	file_export_lib->popup_centered_ratio();
}

#endif // TOOLS_ENABLED
