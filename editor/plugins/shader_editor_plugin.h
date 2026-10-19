// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef SHADER_EDITOR_PLUGIN_H
#define SHADER_EDITOR_PLUGIN_H

#include "editor/editor_plugin.h"

class HSplitContainer;
class ItemList;
class MenuButton;
class ShaderCreateDialog;
class TabContainer;
class TextShaderEditor;
class VisualShaderEditor;

class ShaderEditorPlugin : public EditorPlugin {
	GDCLASS(ShaderEditorPlugin, EditorPlugin);

	struct EditedShader {
		Ref<Shader> shader;
		Ref<ShaderInclude> shader_inc;
		TextShaderEditor *shader_editor = nullptr;
		VisualShaderEditor *visual_shader_editor = nullptr;
	};

	LocalVector<EditedShader> edited_shaders;

	// Always valid operations come first in the enum, file-specific ones
	// should go after FILE_SAVE which is used to build the menu accordingly.
	enum {
		FILE_NEW,
		FILE_NEW_INCLUDE,
		FILE_OPEN,
		FILE_OPEN_INCLUDE,
		FILE_SAVE,
		FILE_SAVE_AS,
		FILE_INSPECT,
		FILE_CLOSE,
		FILE_MAX
	};

	HSplitContainer *main_split = nullptr;
	ItemList *shader_list = nullptr;
	TabContainer *shader_tabs = nullptr;

	Button *button = nullptr;
	MenuButton *file_menu = nullptr;

	ShaderCreateDialog *shader_create_dialog = nullptr;

	void _update_shader_list();
	void _shader_selected(int p_index);
	void _shader_list_clicked(int p_item, Vector2 p_local_mouse_pos, MouseButton p_mouse_button_index);
	void _menu_item_pressed(int p_index);
	void _resource_saved(Object *obj);
	void _close_shader(int p_index);
	void _close_builtin_shaders_from_scene(const String &p_scene);

	void _shader_created(Ref<Shader> p_shader);
	void _shader_include_created(Ref<ShaderInclude> p_shader_inc);
	void _update_shader_list_status();
	void _move_shader_tab(int p_from, int p_to);

	Variant get_drag_data_fw(const Point2 &p_point, Control *p_from);
	bool can_drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from) const;
	void drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from);

protected:
	void _notification(int p_what);

public:
	virtual String get_name() const override { return "Shader"; }
	virtual void edit(Object *p_object) override;
	virtual bool handles(Object *p_object) const override;
	virtual void make_visible(bool p_visible) override;
	virtual void selected_notify() override;

	TextShaderEditor *get_shader_editor(const Ref<Shader> &p_for_shader);
	VisualShaderEditor *get_visual_shader_editor(const Ref<Shader> &p_for_shader);

	virtual void save_external_data() override;
	virtual void apply_changes() override;

	ShaderEditorPlugin();
	~ShaderEditorPlugin();
};

#endif // SHADER_EDITOR_PLUGIN_H
