// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef TEXTURE_RECT_H
#define TEXTURE_RECT_H

#include "scene/gui/control.h"

class TextureRect : public Control {
	GDCLASS(TextureRect, Control);

public:
	enum ExpandMode {
		EXPAND_KEEP_SIZE,
		EXPAND_IGNORE_SIZE,
		EXPAND_FIT_WIDTH,
		EXPAND_FIT_WIDTH_PROPORTIONAL,
		EXPAND_FIT_HEIGHT,
		EXPAND_FIT_HEIGHT_PROPORTIONAL,
	};

	enum StretchMode {
		STRETCH_SCALE,
		STRETCH_TILE,
		STRETCH_KEEP,
		STRETCH_KEEP_CENTERED,
		STRETCH_KEEP_ASPECT,
		STRETCH_KEEP_ASPECT_CENTERED,
		STRETCH_KEEP_ASPECT_COVERED,
	};

private:
	bool hflip = false;
	bool vflip = false;
	Ref<Texture2D> texture;
	ExpandMode expand_mode = EXPAND_KEEP_SIZE;
	StretchMode stretch_mode = STRETCH_SCALE;

	void _texture_changed();

protected:
	void _notification(int p_what);
	virtual Size2 get_minimum_size() const override;
	static void _bind_methods();
#ifndef DISABLE_DEPRECATED
	bool _set(const StringName &p_name, const Variant &p_value);
#endif

public:
	void set_texture(const Ref<Texture2D> &p_tex);
	Ref<Texture2D> get_texture() const;

	void set_expand_mode(ExpandMode p_mode);
	ExpandMode get_expand_mode() const;

	void set_stretch_mode(StretchMode p_mode);
	StretchMode get_stretch_mode() const;

	void set_flip_h(bool p_flip);
	bool is_flipped_h() const;

	void set_flip_v(bool p_flip);
	bool is_flipped_v() const;

	TextureRect();
	~TextureRect();
};

VARIANT_ENUM_CAST(TextureRect::ExpandMode);
VARIANT_ENUM_CAST(TextureRect::StretchMode);

#endif // TEXTURE_RECT_H
