// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef SEPARATOR_H
#define SEPARATOR_H

#include "scene/gui/control.h"
class Separator : public Control {
	GDCLASS(Separator, Control);

	struct ThemeCache {
		int separation = 0;
		Ref<StyleBox> separator_style;
	} theme_cache;

protected:
	Orientation orientation = Orientation::HORIZONTAL;

	virtual void _update_theme_item_cache() override;

	void _notification(int p_what);

public:
	virtual Size2 get_minimum_size() const override;

	Separator();
	~Separator();
};

class VSeparator : public Separator {
	GDCLASS(VSeparator, Separator);

public:
	VSeparator();
};

class HSeparator : public Separator {
	GDCLASS(HSeparator, Separator);

public:
	HSeparator();
};

#endif // SEPARATOR_H
