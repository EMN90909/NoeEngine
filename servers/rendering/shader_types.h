// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef SHADER_TYPES_H
#define SHADER_TYPES_H

#include "core/templates/rb_map.h"
#include "servers/rendering_server.h"
#include "shader_language.h"

class ShaderTypes {
	struct Type {
		HashMap<StringName, ShaderLanguage::FunctionInfo> functions;
		Vector<ShaderLanguage::ModeInfo> modes;
	};

	HashMap<RS::ShaderMode, Type> shader_modes;

	static ShaderTypes *singleton;

	HashSet<String> shader_types;
	List<String> shader_types_list;

public:
	static ShaderTypes *get_singleton() { return singleton; }

	const HashMap<StringName, ShaderLanguage::FunctionInfo> &get_functions(RS::ShaderMode p_mode) const;
	const Vector<ShaderLanguage::ModeInfo> &get_modes(RS::ShaderMode p_mode) const;
	const HashSet<String> &get_types() const;
	const List<String> &get_types_list() const;

	ShaderTypes();
};

#endif // SHADER_TYPES_H
