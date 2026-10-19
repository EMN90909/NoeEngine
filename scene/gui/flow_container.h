// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef FLOW_CONTAINER_H
#define FLOW_CONTAINER_H

#include "scene/gui/container.h"

class FlowContainer : public Container {
	GDCLASS(FlowContainer, Container);

public:
	enum AlignmentMode {
		ALIGNMENT_BEGIN,
		ALIGNMENT_CENTER,
		ALIGNMENT_END
	};

private:
	int cached_size = 0;
	int cached_line_count = 0;

	bool vertical = false;
	AlignmentMode alignment = ALIGNMENT_BEGIN;

	struct ThemeCache {
		int h_separation = 0;
		int v_separation = 0;
	} theme_cache;

	void _resort();

protected:
	bool is_fixed = false;

	virtual void _update_theme_item_cache() override;

	void _notification(int p_what);
	void _validate_property(PropertyInfo &p_property) const;
	static void _bind_methods();

public:
	int get_line_count() const;

	void set_alignment(AlignmentMode p_alignment);
	AlignmentMode get_alignment() const;

	void set_vertical(bool p_vertical);
	bool is_vertical() const;

	virtual Size2 get_minimum_size() const override;

	virtual Vector<int> get_allowed_size_flags_horizontal() const override;
	virtual Vector<int> get_allowed_size_flags_vertical() const override;

	FlowContainer(bool p_vertical = false);
};

class HFlowContainer : public FlowContainer {
	GDCLASS(HFlowContainer, FlowContainer);

public:
	HFlowContainer() :
			FlowContainer(false) { is_fixed = true; }
};

class VFlowContainer : public FlowContainer {
	GDCLASS(VFlowContainer, FlowContainer);

public:
	VFlowContainer() :
			FlowContainer(true) { is_fixed = true; }
};

VARIANT_ENUM_CAST(FlowContainer::AlignmentMode);

#endif // FLOW_CONTAINER_H
