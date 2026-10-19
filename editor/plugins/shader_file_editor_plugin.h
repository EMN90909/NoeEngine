// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef SHADER_FILE_EDITOR_PLUGIN_H
#define SHADER_FILE_EDITOR_PLUGIN_H

#include "editor/code_editor.h"
#include "editor/editor_plugin.h"
#include "scene/gui/menu_button.h"
#include "scene/gui/panel_container.h"
#include "scene/gui/rich_text_label.h"
#include "scene/gui/tab_container.h"
#include "scene/gui/text_edit.h"
#include "scene/main/timer.h"
#include "servers/rendering/rendering_device_binds.h"

class ItemList;

class ShaderFileEditor : public PanelContainer {
	GDCLASS(ShaderFileEditor, PanelContainer);

	Ref<RDShaderFile> shader_file;

	HBoxContainer *stage_hb = nullptr;
	ItemList *versions = nullptr;
	Button *stages[RD::SHADER_STAGE_MAX];
	RichTextLabel *error_text = nullptr;

	void _update_version(const StringName &p_version_txt, const RenderingDevice::ShaderStage p_stage);
	void _version_selected(int p_stage);
	void _editor_settings_changed();

	void _update_options();
	void _shader_changed();

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	static ShaderFileEditor *singleton;
	void edit(const Ref<RDShaderFile> &p_shader);

	ShaderFileEditor();
};

class ShaderFileEditorPlugin : public EditorPlugin {
	GDCLASS(ShaderFileEditorPlugin, EditorPlugin);

	ShaderFileEditor *shader_editor = nullptr;
	Button *button = nullptr;

public:
	virtual String get_name() const override { return "ShaderFile"; }
	bool has_main_screen() const override { return false; }
	virtual void edit(Object *p_object) override;
	virtual bool handles(Object *p_object) const override;
	virtual void make_visible(bool p_visible) override;

	ShaderFileEditor *get_shader_editor() const { return shader_editor; }

	ShaderFileEditorPlugin();
	~ShaderFileEditorPlugin();
};

#endif // SHADER_FILE_EDITOR_PLUGIN_H
