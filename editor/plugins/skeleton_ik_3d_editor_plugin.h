// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef SKELETON_IK_3D_EDITOR_PLUGIN_H
#define SKELETON_IK_3D_EDITOR_PLUGIN_H

#include "editor/editor_plugin.h"

class SkeletonIK3D;

class SkeletonIK3DEditorPlugin : public EditorPlugin {
	GDCLASS(SkeletonIK3DEditorPlugin, EditorPlugin);

	SkeletonIK3D *skeleton_ik = nullptr;

	Button *play_btn = nullptr;

	void _play();

protected:
	static void _bind_methods();

public:
	virtual String get_name() const override { return "SkeletonIK3D"; }
	bool has_main_screen() const override { return false; }
	virtual void edit(Object *p_object) override;
	virtual bool handles(Object *p_object) const override;
	virtual void make_visible(bool p_visible) override;

	SkeletonIK3DEditorPlugin();
	~SkeletonIK3DEditorPlugin();
};

#endif // SKELETON_IK_3D_EDITOR_PLUGIN_H
