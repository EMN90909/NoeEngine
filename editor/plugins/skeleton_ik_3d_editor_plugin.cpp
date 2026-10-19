// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "skeleton_ik_3d_editor_plugin.h"

#include "editor/editor_node.h"
#include "scene/3d/skeleton_ik_3d.h"

void SkeletonIK3DEditorPlugin::_play() {
	if (!skeleton_ik) {
		return;
	}

	if (!skeleton_ik->get_parent_skeleton()) {
		return;
	}

	if (play_btn->is_pressed()) {
		skeleton_ik->start();
	} else {
		skeleton_ik->stop();
		skeleton_ik->get_parent_skeleton()->clear_bones_global_pose_override();
	}
}

void SkeletonIK3DEditorPlugin::edit(Object *p_object) {
	if (p_object != skeleton_ik) {
		if (skeleton_ik) {
			play_btn->set_pressed(false);
			_play();
		}
	}

	SkeletonIK3D *s = Object::cast_to<SkeletonIK3D>(p_object);
	if (!s) {
		return;
	}

	skeleton_ik = s;
}

bool SkeletonIK3DEditorPlugin::handles(Object *p_object) const {
	return p_object->is_class("SkeletonIK3D");
}

void SkeletonIK3DEditorPlugin::make_visible(bool p_visible) {
	if (p_visible) {
		play_btn->show();
	} else {
		play_btn->hide();
	}
}

void SkeletonIK3DEditorPlugin::_bind_methods() {
}

SkeletonIK3DEditorPlugin::SkeletonIK3DEditorPlugin() {
	play_btn = memnew(Button);
	play_btn->set_icon(EditorNode::get_singleton()->get_gui_base()->get_theme_icon(SNAME("Play"), SNAME("EditorIcons")));
	play_btn->set_text(TTR("Play IK"));
	play_btn->set_toggle_mode(true);
	play_btn->hide();
	play_btn->connect("pressed", callable_mp(this, &SkeletonIK3DEditorPlugin::_play));
	add_control_to_container(CONTAINER_SPATIAL_EDITOR_MENU, play_btn);
	skeleton_ik = nullptr;
}

SkeletonIK3DEditorPlugin::~SkeletonIK3DEditorPlugin() {}
