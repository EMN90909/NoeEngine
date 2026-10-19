// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef SKELETON_MODIFICATION_2D_STACKHOLDER_H
#define SKELETON_MODIFICATION_2D_STACKHOLDER_H

#include "scene/2d/skeleton_2d.h"
#include "scene/resources/skeleton_modification_2d.h"

///////////////////////////////////////
// SkeletonModification2DJIGGLE
///////////////////////////////////////

class SkeletonModification2DStackHolder : public SkeletonModification2D {
	GDCLASS(SkeletonModification2DStackHolder, SkeletonModification2D);

protected:
	static void _bind_methods();
	bool _get(const StringName &p_path, Variant &r_ret) const;
	bool _set(const StringName &p_path, const Variant &p_value);
	void _get_property_list(List<PropertyInfo> *p_list) const;

public:
	Ref<SkeletonModificationStack2D> held_modification_stack;

	void _execute(float p_delta) override;
	void _setup_modification(SkeletonModificationStack2D *p_stack) override;
	void _draw_editor_gizmo() override;

	void set_held_modification_stack(Ref<SkeletonModificationStack2D> p_held_stack);
	Ref<SkeletonModificationStack2D> get_held_modification_stack() const;

	SkeletonModification2DStackHolder();
	~SkeletonModification2DStackHolder();
};

#endif // SKELETON_MODIFICATION_2D_STACKHOLDER_H
