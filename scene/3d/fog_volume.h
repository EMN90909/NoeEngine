// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef FOG_VOLUME_H
#define FOG_VOLUME_H

#include "core/templates/rid.h"
#include "scene/3d/visual_instance_3d.h"
#include "scene/main/node.h"
#include "scene/main/viewport.h"
#include "scene/resources/material.h"

class FogVolume : public VisualInstance3D {
	GDCLASS(FogVolume, VisualInstance3D);

	Vector3 size = Vector3(2, 2, 2);
	Ref<Material> material;
	RS::FogVolumeShape shape = RS::FOG_VOLUME_SHAPE_BOX;

	RID volume;

protected:
	_FORCE_INLINE_ RID _get_volume() { return volume; }
	static void _bind_methods();
	void _validate_property(PropertyInfo &p_property) const;
#ifndef DISABLE_DEPRECATED
	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_property) const;
#endif // DISABLE_DEPRECATED

public:
	void set_size(const Vector3 &p_size);
	Vector3 get_size() const;

	void set_shape(RS::FogVolumeShape p_type);
	RS::FogVolumeShape get_shape() const;

	void set_material(const Ref<Material> &p_material);
	Ref<Material> get_material() const;

	virtual AABB get_aabb() const override;
	PackedStringArray get_configuration_warnings() const override;

	FogVolume();
	~FogVolume();
};

#endif // FOG_VOLUME_H
