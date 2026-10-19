// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "editor_native_shader_source_visualizer.h"

#include "scene/gui/text_edit.h"

void EditorNativeShaderSourceVisualizer::_inspect_shader(RID p_shader) {
	if (versions) {
		memdelete(versions);
		versions = nullptr;
	}

	RS::ShaderNativeSourceCode nsc = RS::get_singleton()->shader_get_native_source_code(p_shader);

	versions = memnew(TabContainer);
	versions->set_tab_alignment(TabBar::ALIGNMENT_CENTER);
	versions->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	versions->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	for (int i = 0; i < nsc.versions.size(); i++) {
		TabContainer *vtab = memnew(TabContainer);
		vtab->set_name("Version " + itos(i));
		vtab->set_tab_alignment(TabBar::ALIGNMENT_CENTER);
		vtab->set_v_size_flags(Control::SIZE_EXPAND_FILL);
		vtab->set_h_size_flags(Control::SIZE_EXPAND_FILL);
		versions->add_child(vtab);
		for (int j = 0; j < nsc.versions[i].stages.size(); j++) {
			TextEdit *vtext = memnew(TextEdit);
			vtext->set_editable(false);
			vtext->set_name(nsc.versions[i].stages[j].name);
			vtext->set_text(nsc.versions[i].stages[j].code);
			vtext->set_v_size_flags(Control::SIZE_EXPAND_FILL);
			vtext->set_h_size_flags(Control::SIZE_EXPAND_FILL);
			vtab->add_child(vtext);
		}
	}
	add_child(versions);
	popup_centered_ratio();
}

void EditorNativeShaderSourceVisualizer::_bind_methods() {
	ClassDB::bind_method("_inspect_shader", &EditorNativeShaderSourceVisualizer::_inspect_shader);
}
EditorNativeShaderSourceVisualizer::EditorNativeShaderSourceVisualizer() {
	add_to_group("_native_shader_source_visualizer");
	set_title(TTR("Native Shader Source Inspector"));
}
