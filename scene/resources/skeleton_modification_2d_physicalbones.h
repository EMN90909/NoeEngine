// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef SKELETON_MODIFICATION_2D_PHYSICALBONES_H
#define SKELETON_MODIFICATION_2D_PHYSICALBONES_H

#include "scene/2d/skeleton_2d.h"
#include "scene/resources/skeleton_modification_2d.h"

///////////////////////////////////////
// SkeletonModification2DJIGGLE
///////////////////////////////////////

class SkeletonModification2DPhysicalBones : public SkeletonModification2D {
	GDCLASS(SkeletonModification2DPhysicalBones, SkeletonModification2D);

private:
	struct PhysicalBone_Data2D {
		NodePath physical_bone_node;
		ObjectID physical_bone_node_cache;
	};
	Vector<PhysicalBone_Data2D> physical_bone_chain;

	void _physical_bone_update_cache(int p_joint_idx);

	bool _simulation_state_dirty = false;
	TypedArray<StringName> _simulation_state_dirty_names;
	bool _simulation_state_dirty_process = false;
	void _update_simulation_state();

protected:
	static void _bind_methods();
	bool _get(const StringName &p_path, Variant &r_ret) const;
	bool _set(const StringName &p_path, const Variant &p_value);
	void _get_property_list(List<PropertyInfo> *p_list) const;

public:
	void _execute(float p_delta) override;
	void _setup_modification(SkeletonModificationStack2D *p_stack) override;

	int get_physical_bone_chain_length();
	void set_physical_bone_chain_length(int p_new_length);

	void set_physical_bone_node(int p_joint_idx, const NodePath &p_path);
	NodePath get_physical_bone_node(int p_joint_idx) const;

	void fetch_physical_bones();
	void start_simulation(const TypedArray<StringName> &p_bones);
	void stop_simulation(const TypedArray<StringName> &p_bones);

	SkeletonModification2DPhysicalBones();
	~SkeletonModification2DPhysicalBones();
};

#endif // SKELETON_MODIFICATION_2D_PHYSICALBONES_H
