// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef ANIMATION_BLEND_SPACE_1D_H
#define ANIMATION_BLEND_SPACE_1D_H

#include "scene/animation/animation_tree.h"

class AnimationNodeBlendSpace1D : public AnimationRootNode {
	GDCLASS(AnimationNodeBlendSpace1D, AnimationRootNode);

public:
	enum BlendMode {
		BLEND_MODE_INTERPOLATED,
		BLEND_MODE_DISCRETE,
		BLEND_MODE_DISCRETE_CARRY,
	};

protected:
	enum {
		MAX_BLEND_POINTS = 64
	};

	struct BlendPoint {
		StringName name;
		Ref<AnimationRootNode> node;
		float position = 0.0;
	};

	BlendPoint blend_points[MAX_BLEND_POINTS];
	int blend_points_used = 0;

	float max_space = 1.0;
	float min_space = -1.0;

	float snap = 0.1;

	String value_label = "value";

	void _add_blend_point(int p_index, const Ref<AnimationRootNode> &p_node);

	StringName blend_position = "blend_position";
	StringName closest = "closest";
	StringName length_internal = "length_internal";

	BlendMode blend_mode = BLEND_MODE_INTERPOLATED;

	bool sync = false;

	void _validate_property(PropertyInfo &p_property) const;
	static void _bind_methods();

	virtual void _tree_changed() override;
	virtual void _animation_node_renamed(const ObjectID &p_oid, const String &p_old_name, const String &p_new_name) override;
	virtual void _animation_node_removed(const ObjectID &p_oid, const StringName &p_node) override;

public:
	virtual void get_parameter_list(List<PropertyInfo> *r_list) const override;
	virtual Variant get_parameter_default_value(const StringName &p_parameter) const override;

	virtual void get_child_nodes(List<ChildNode> *r_child_nodes) override;

	void add_blend_point(const Ref<AnimationRootNode> &p_node, float p_position, int p_at_index = -1);
	void set_blend_point_position(int p_point, float p_position);
	void set_blend_point_node(int p_point, const Ref<AnimationRootNode> &p_node);

	float get_blend_point_position(int p_point) const;
	Ref<AnimationRootNode> get_blend_point_node(int p_point) const;
	void remove_blend_point(int p_point);
	int get_blend_point_count() const;

	void set_min_space(float p_min);
	float get_min_space() const;

	void set_max_space(float p_max);
	float get_max_space() const;

	void set_snap(float p_snap);
	float get_snap() const;

	void set_value_label(const String &p_label);
	String get_value_label() const;

	void set_blend_mode(BlendMode p_blend_mode);
	BlendMode get_blend_mode() const;

	void set_use_sync(bool p_sync);
	bool is_using_sync() const;

	double process(double p_time, bool p_seek, bool p_is_external_seeking) override;
	String get_caption() const override;

	Ref<AnimationNode> get_child_by_name(const StringName &p_name) override;

	AnimationNodeBlendSpace1D();
	~AnimationNodeBlendSpace1D();
};

VARIANT_ENUM_CAST(AnimationNodeBlendSpace1D::BlendMode)

#endif // ANIMATION_BLEND_SPACE_1D_H
