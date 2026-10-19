// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef MISSING_NODE_H
#define MISSING_NODE_H

#include "core/io/missing_resource.h"
#include "scene/main/node.h"

class MissingNode : public Node {
	GDCLASS(MissingNode, Node)
	HashMap<StringName, Variant> properties;

	String original_class;
	bool recording_properties = false;

protected:
	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;

	static void _bind_methods();

public:
	void set_original_class(const String &p_class);
	String get_original_class() const;

	void set_recording_properties(bool p_enable);
	bool is_recording_properties() const;

	virtual PackedStringArray get_configuration_warnings() const override;

	MissingNode();
};

#endif // MISSING_NODE_H
