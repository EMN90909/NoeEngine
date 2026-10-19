// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef PARALLAX_BACKGROUND_H
#define PARALLAX_BACKGROUND_H

#include "scene/main/canvas_layer.h"

class ParallaxBackground : public CanvasLayer {
	GDCLASS(ParallaxBackground, CanvasLayer);

	Point2 offset;
	real_t scale = 1.0;
	Point2 base_offset;
	Point2 base_scale = Vector2(1, 1);
	Point2 screen_offset;
	String group_name;
	Point2 limit_begin;
	Point2 limit_end;
	Point2 final_offset;
	bool ignore_camera_zoom = false;

	void _update_scroll();

protected:
	void _camera_moved(const Transform2D &p_transform, const Point2 &p_screen_offset);

	void _notification(int p_what);
	static void _bind_methods();

public:
	void set_scroll_offset(const Point2 &p_ofs);
	Point2 get_scroll_offset() const;

	void set_scroll_scale(real_t p_scale);
	real_t get_scroll_scale() const;

	void set_scroll_base_offset(const Point2 &p_ofs);
	Point2 get_scroll_base_offset() const;

	void set_scroll_base_scale(const Point2 &p_ofs);
	Point2 get_scroll_base_scale() const;

	void set_limit_begin(const Point2 &p_ofs);
	Point2 get_limit_begin() const;

	void set_limit_end(const Point2 &p_ofs);
	Point2 get_limit_end() const;

	void set_ignore_camera_zoom(bool ignore);
	bool is_ignore_camera_zoom();

	Vector2 get_final_offset() const;

	ParallaxBackground();
};

#endif // PARALLAX_BACKGROUND_H
