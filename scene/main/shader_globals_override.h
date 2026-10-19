// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef SHADER_GLOBALS_OVERRIDE_H
#define SHADER_GLOBALS_OVERRIDE_H

#include "scene/main/node.h"

class ShaderGlobalsOverride : public Node {
	GDCLASS(ShaderGlobalsOverride, Node);

	struct Override {
		bool in_use = false;
		Variant override;
	};

	StringName *_remap(const StringName &p_name) const;

	bool active = false;
	mutable HashMap<StringName, Override> overrides;
	mutable HashMap<StringName, StringName> param_remaps;

	void _activate();

protected:
	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;

	void _notification(int p_what);
	static void _bind_methods();

public:
	PackedStringArray get_configuration_warnings() const override;

	ShaderGlobalsOverride();
};

#endif // SHADER_GLOBALS_OVERRIDE_H
