// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef COLOR_MODE_H
#define COLOR_MODE_H

#include "scene/gui/color_picker.h"

struct Color;

class ColorMode {
public:
	ColorPicker *color_picker = nullptr;

	virtual String get_name() const = 0;

	virtual int get_slider_count() const { return 3; };
	virtual float get_slider_step() const = 0;
	virtual String get_slider_label(int idx) const = 0;
	virtual float get_slider_max(int idx) const = 0;
	virtual float get_slider_value(int idx) const = 0;

	virtual Color get_color() const = 0;

	virtual void slider_draw(int p_which) = 0;
	virtual bool apply_theme() const { return false; }
	virtual ColorPicker::PickerShapeType get_shape_override() const { return ColorPicker::SHAPE_MAX; }

	ColorMode(ColorPicker *p_color_picker);
	virtual ~ColorMode(){};
};

class ColorModeHSV : public ColorMode {
public:
	String labels[3] = { "H", "S", "V" };
	float slider_max[4] = { 359, 100, 100, 255 };

	virtual String get_name() const override { return "HSV"; }

	virtual float get_slider_step() const override { return 1.0; }
	virtual String get_slider_label(int idx) const override;
	virtual float get_slider_max(int idx) const override;
	virtual float get_slider_value(int idx) const override;

	virtual Color get_color() const override;

	virtual void slider_draw(int p_which) override;

	ColorModeHSV(ColorPicker *p_color_picker) :
			ColorMode(p_color_picker){};
};

class ColorModeRGB : public ColorMode {
public:
	String labels[3] = { "R", "G", "B" };

	virtual String get_name() const override { return "RGB"; }

	virtual float get_slider_step() const override { return 1; }
	virtual String get_slider_label(int idx) const override;
	virtual float get_slider_max(int idx) const override;
	virtual float get_slider_value(int idx) const override;

	virtual Color get_color() const override;

	virtual void slider_draw(int p_which) override;

	ColorModeRGB(ColorPicker *p_color_picker) :
			ColorMode(p_color_picker){};
};

class ColorModeRAW : public ColorMode {
public:
	String labels[3] = { "R", "G", "B" };
	float slider_max[4] = { 100, 100, 100, 1 };

	virtual String get_name() const override { return "RAW"; }

	virtual float get_slider_step() const override { return 0.01; }
	virtual String get_slider_label(int idx) const override;
	virtual float get_slider_max(int idx) const override;
	virtual float get_slider_value(int idx) const override;

	virtual Color get_color() const override;

	virtual void slider_draw(int p_which) override;
	virtual bool apply_theme() const override;

	ColorModeRAW(ColorPicker *p_color_picker) :
			ColorMode(p_color_picker){};
};

class ColorModeOKHSL : public ColorMode {
public:
	String labels[3] = { "H", "S", "L" };
	float slider_max[4] = { 359, 100, 100, 255 };

	virtual String get_name() const override { return "OKHSL"; }

	virtual float get_slider_step() const override { return 1.0; }
	virtual String get_slider_label(int idx) const override;
	virtual float get_slider_max(int idx) const override;
	virtual float get_slider_value(int idx) const override;

	virtual Color get_color() const override;

	virtual void slider_draw(int p_which) override;
	virtual ColorPicker::PickerShapeType get_shape_override() const override { return ColorPicker::SHAPE_OKHSL_CIRCLE; }

	ColorModeOKHSL(ColorPicker *p_color_picker) :
			ColorMode(p_color_picker){};

	~ColorModeOKHSL(){};
};

#endif // COLOR_MODE_H
