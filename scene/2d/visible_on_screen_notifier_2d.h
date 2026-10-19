// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef VISIBLE_ON_SCREEN_NOTIFIER_2D_H
#define VISIBLE_ON_SCREEN_NOTIFIER_2D_H

#include "scene/2d/node_2d.h"

class Viewport;
class VisibleOnScreenNotifier2D : public Node2D {
	GDCLASS(VisibleOnScreenNotifier2D, Node2D);

	HashSet<Viewport *> viewports;

	Rect2 rect;

private:
	bool on_screen = false;
	void _visibility_enter();
	void _visibility_exit();

protected:
	virtual void _screen_enter() {}
	virtual void _screen_exit() {}

	void _notification(int p_what);
	static void _bind_methods();

public:
#ifdef TOOLS_ENABLED
	virtual Rect2 _edit_get_rect() const override;
	virtual bool _edit_use_rect() const override;
#endif

	void set_rect(const Rect2 &p_rect);
	Rect2 get_rect() const;

	bool is_on_screen() const;

	VisibleOnScreenNotifier2D();
};

class VisibleOnScreenEnabler2D : public VisibleOnScreenNotifier2D {
	GDCLASS(VisibleOnScreenEnabler2D, VisibleOnScreenNotifier2D);

public:
	enum EnableMode {
		ENABLE_MODE_INHERIT,
		ENABLE_MODE_ALWAYS,
		ENABLE_MODE_WHEN_PAUSED,
	};

protected:
	ObjectID node_id;
	virtual void _screen_enter() override;
	virtual void _screen_exit() override;

	EnableMode enable_mode = ENABLE_MODE_INHERIT;
	NodePath enable_node_path = NodePath("..");

	void _notification(int p_what);
	static void _bind_methods();

	void _update_enable_mode(bool p_enable);

public:
	void set_enable_mode(EnableMode p_mode);
	EnableMode get_enable_mode();

	void set_enable_node_path(NodePath p_path);
	NodePath get_enable_node_path();

	VisibleOnScreenEnabler2D();
};

VARIANT_ENUM_CAST(VisibleOnScreenEnabler2D::EnableMode);

#endif // VISIBLE_ON_SCREEN_NOTIFIER_2D_H
