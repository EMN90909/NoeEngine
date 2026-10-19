// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef SKELETON_MODIFICATION_STACK_2D_H
#define SKELETON_MODIFICATION_STACK_2D_H

#include "scene/2d/skeleton_2d.h"
#include "scene/resources/skeleton_modification_2d.h"

///////////////////////////////////////
// SkeletonModificationStack2D
///////////////////////////////////////

class Skeleton2D;
class SkeletonModification2D;
class Bone2D;

class SkeletonModificationStack2D : public Resource {
	GDCLASS(SkeletonModificationStack2D, Resource);
	friend class Skeleton2D;
	friend class SkeletonModification2D;

protected:
	static void _bind_methods();
	void _get_property_list(List<PropertyInfo> *p_list) const;
	bool _set(const StringName &p_path, const Variant &p_value);
	bool _get(const StringName &p_path, Variant &r_ret) const;

public:
	Skeleton2D *skeleton = nullptr;
	bool is_setup = false;
	bool enabled = false;
	float strength = 1.0;

	enum EXECUTION_MODE {
		execution_mode_process,
		execution_mode_physics_process
	};

	Vector<Ref<SkeletonModification2D>> modifications = Vector<Ref<SkeletonModification2D>>();

	void setup();
	void execute(float p_delta, int p_execution_mode);

	bool editor_gizmo_dirty = false;
	void draw_editor_gizmos();
	void set_editor_gizmos_dirty(bool p_dirty);

	void enable_all_modifications(bool p_enable);
	Ref<SkeletonModification2D> get_modification(int p_mod_idx) const;
	void add_modification(Ref<SkeletonModification2D> p_mod);
	void delete_modification(int p_mod_idx);
	void set_modification(int p_mod_idx, Ref<SkeletonModification2D> p_mod);

	void set_modification_count(int p_count);
	int get_modification_count() const;

	void set_skeleton(Skeleton2D *p_skeleton);
	Skeleton2D *get_skeleton() const;

	bool get_is_setup() const;

	void set_enabled(bool p_enabled);
	bool get_enabled() const;

	void set_strength(float p_strength);
	float get_strength() const;

	SkeletonModificationStack2D();
};

#endif // SKELETON_MODIFICATION_STACK_2D_H
