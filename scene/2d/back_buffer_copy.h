// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef BACK_BUFFER_COPY_H
#define BACK_BUFFER_COPY_H

#include "scene/2d/node_2d.h"

class BackBufferCopy : public Node2D {
	GDCLASS(BackBufferCopy, Node2D);

public:
	enum CopyMode {
		COPY_MODE_DISABLED,
		COPY_MODE_RECT,
		COPY_MODE_VIEWPORT
	};

private:
	Rect2 rect = Rect2(-100, -100, 200, 200);
	CopyMode copy_mode = COPY_MODE_RECT;

	void _update_copy_mode();

protected:
	static void _bind_methods();
	void _validate_property(PropertyInfo &p_property) const;

public:
#ifdef TOOLS_ENABLED
	Rect2 _edit_get_rect() const override;
	virtual bool _edit_use_rect() const override;
#endif

	void set_rect(const Rect2 &p_rect);
	Rect2 get_rect() const;
	Rect2 get_anchorable_rect() const override;

	void set_copy_mode(CopyMode p_mode);
	CopyMode get_copy_mode() const;

	BackBufferCopy();
	~BackBufferCopy();
};

VARIANT_ENUM_CAST(BackBufferCopy::CopyMode);

#endif // BACK_BUFFER_COPY_H
