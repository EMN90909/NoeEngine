// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef ASPECT_RATIO_CONTAINER_H
#define ASPECT_RATIO_CONTAINER_H

#include "scene/gui/container.h"

class AspectRatioContainer : public Container {
	GDCLASS(AspectRatioContainer, Container);

protected:
	void _notification(int p_what);
	static void _bind_methods();
	virtual Size2 get_minimum_size() const override;

public:
	enum StretchMode {
		STRETCH_WIDTH_CONTROLS_HEIGHT,
		STRETCH_HEIGHT_CONTROLS_WIDTH,
		STRETCH_FIT,
		STRETCH_COVER,
	};
	enum AlignmentMode {
		ALIGNMENT_BEGIN,
		ALIGNMENT_CENTER,
		ALIGNMENT_END,
	};

private:
	float ratio = 1.0;
	StretchMode stretch_mode = STRETCH_FIT;
	AlignmentMode alignment_horizontal = ALIGNMENT_CENTER;
	AlignmentMode alignment_vertical = ALIGNMENT_CENTER;

public:
	void set_ratio(float p_ratio);
	float get_ratio() const { return ratio; }

	void set_stretch_mode(StretchMode p_mode);
	StretchMode get_stretch_mode() const { return stretch_mode; }

	void set_alignment_horizontal(AlignmentMode p_alignment_horizontal);
	AlignmentMode get_alignment_horizontal() const { return alignment_horizontal; }

	void set_alignment_vertical(AlignmentMode p_alignment_vertical);
	AlignmentMode get_alignment_vertical() const { return alignment_vertical; }

	virtual Vector<int> get_allowed_size_flags_horizontal() const override;
	virtual Vector<int> get_allowed_size_flags_vertical() const override;
};

VARIANT_ENUM_CAST(AspectRatioContainer::StretchMode);
VARIANT_ENUM_CAST(AspectRatioContainer::AlignmentMode);

#endif // ASPECT_RATIO_CONTAINER_H
