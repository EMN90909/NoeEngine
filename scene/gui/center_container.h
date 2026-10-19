// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef CENTER_CONTAINER_H
#define CENTER_CONTAINER_H

#include "scene/gui/container.h"

class CenterContainer : public Container {
	GDCLASS(CenterContainer, Container);

	bool use_top_left = false;

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	void set_use_top_left(bool p_enable);
	bool is_using_top_left() const;

	virtual Size2 get_minimum_size() const override;

	virtual Vector<int> get_allowed_size_flags_horizontal() const override;
	virtual Vector<int> get_allowed_size_flags_vertical() const override;

	CenterContainer();
};

#endif // CENTER_CONTAINER_H
