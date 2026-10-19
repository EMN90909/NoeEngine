// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef PHYSICAL_BONE_3D_EDITOR_PLUGIN_H
#define PHYSICAL_BONE_3D_EDITOR_PLUGIN_H

#include "editor/editor_plugin.h"
#include "scene/3d/physics_body_3d.h"
#include "scene/gui/box_container.h"
#include "scene/gui/button.h"

class PhysicalBone3DEditor : public Object {
	GDCLASS(PhysicalBone3DEditor, Object);

	HBoxContainer *spatial_editor_hb = nullptr;
	Button *button_transform_joint = nullptr;

	PhysicalBone3D *selected = nullptr;

protected:
	static void _bind_methods();

private:
	void _on_toggle_button_transform_joint(bool p_is_pressed);
	void _set_move_joint();

public:
	PhysicalBone3DEditor();
	~PhysicalBone3DEditor() {}

	void set_selected(PhysicalBone3D *p_pb);

	void hide();
	void show();
};

class PhysicalBone3DEditorPlugin : public EditorPlugin {
	GDCLASS(PhysicalBone3DEditorPlugin, EditorPlugin);

	PhysicalBone3D *selected = nullptr;
	PhysicalBone3DEditor physical_bone_editor;

public:
	virtual String get_name() const override { return "PhysicalBone3D"; }
	virtual bool handles(Object *p_object) const override { return p_object->is_class("PhysicalBone3D"); }
	virtual void make_visible(bool p_visible) override;
	virtual void edit(Object *p_node) override;

	PhysicalBone3DEditorPlugin();
};

#endif // PHYSICAL_BONE_3D_EDITOR_PLUGIN_H
