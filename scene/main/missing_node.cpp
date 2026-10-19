// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "missing_node.h"

bool MissingNode::_set(const StringName &p_name, const Variant &p_value) {
	if (is_recording_properties()) {
		properties.insert(p_name, p_value);
		return true; //always valid to set (add)
	} else {
		if (!properties.has(p_name)) {
			return false;
		}

		properties[p_name] = p_value;
		return true;
	}
}

bool MissingNode::_get(const StringName &p_name, Variant &r_ret) const {
	if (!properties.has(p_name)) {
		return false;
	}
	r_ret = properties[p_name];
	return true;
}

void MissingNode::_get_property_list(List<PropertyInfo> *p_list) const {
	for (const KeyValue<StringName, Variant> &E : properties) {
		p_list->push_back(PropertyInfo(E.value.get_type(), E.key));
	}
}

void MissingNode::set_original_class(const String &p_class) {
	original_class = p_class;
}

String MissingNode::get_original_class() const {
	return original_class;
}

void MissingNode::set_recording_properties(bool p_enable) {
	recording_properties = p_enable;
}

bool MissingNode::is_recording_properties() const {
	return recording_properties;
}

PackedStringArray MissingNode::get_configuration_warnings() const {
	// The mere existence of this node is warning.
	PackedStringArray ret;
	ret.push_back(vformat(RTR("This node was saved as class type '%s', which was no longer available when this scene was loaded."), original_class));
	ret.push_back(RTR("Data from the original node is kept as a placeholder until this type of node is available again. It can hence be safely re-saved without risk of data loss."));
	return ret;
}

void MissingNode::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_original_class", "name"), &MissingNode::set_original_class);
	ClassDB::bind_method(D_METHOD("get_original_class"), &MissingNode::get_original_class);

	ClassDB::bind_method(D_METHOD("set_recording_properties", "enable"), &MissingNode::set_recording_properties);
	ClassDB::bind_method(D_METHOD("is_recording_properties"), &MissingNode::is_recording_properties);

	// Expose, but not save.
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "original_class", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "set_original_class", "get_original_class");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "recording_properties", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "set_recording_properties", "is_recording_properties");
}

MissingNode::MissingNode() {
}
