// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "instance_placeholder.h"

#include "core/io/resource_loader.h"
#include "scene/resources/packed_scene.h"

bool InstancePlaceholder::_set(const StringName &p_name, const Variant &p_value) {
	PropSet ps;
	ps.name = p_name;
	ps.value = p_value;
	stored_values.push_back(ps);
	return true;
}

bool InstancePlaceholder::_get(const StringName &p_name, Variant &r_ret) const {
	for (const PropSet &E : stored_values) {
		if (E.name == p_name) {
			r_ret = E.value;
			return true;
		}
	}
	return false;
}

void InstancePlaceholder::_get_property_list(List<PropertyInfo> *p_list) const {
	for (const PropSet &E : stored_values) {
		PropertyInfo pi;
		pi.name = E.name;
		pi.type = E.value.get_type();
		pi.usage = PROPERTY_USAGE_STORAGE;

		p_list->push_back(pi);
	}
}

void InstancePlaceholder::set_instance_path(const String &p_name) {
	path = p_name;
}

String InstancePlaceholder::get_instance_path() const {
	return path;
}

Node *InstancePlaceholder::create_instance(bool p_replace, const Ref<PackedScene> &p_custom_scene) {
	ERR_FAIL_COND_V(!is_inside_tree(), nullptr);

	Node *base = get_parent();
	if (!base) {
		return nullptr;
	}

	Ref<PackedScene> ps;
	if (p_custom_scene.is_valid()) {
		ps = p_custom_scene;
	} else {
		ps = ResourceLoader::load(path, "PackedScene");
	}

	if (!ps.is_valid()) {
		return nullptr;
	}
	Node *scene = ps->instantiate();
	if (!scene) {
		return nullptr;
	}
	scene->set_name(get_name());
	int pos = get_index();

	for (const PropSet &E : stored_values) {
		scene->set(E.name, E.value);
	}

	if (p_replace) {
		queue_free();
		base->remove_child(this);
	}

	base->add_child(scene);
	base->move_child(scene, pos);

	return scene;
}

Dictionary InstancePlaceholder::get_stored_values(bool p_with_order) {
	Dictionary ret;
	PackedStringArray order;

	for (const PropSet &E : stored_values) {
		ret[E.name] = E.value;
		if (p_with_order) {
			order.push_back(E.name);
		}
	};

	if (p_with_order) {
		ret[".order"] = order;
	}

	return ret;
};

void InstancePlaceholder::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_stored_values", "with_order"), &InstancePlaceholder::get_stored_values, DEFVAL(false));
	ClassDB::bind_method(D_METHOD("create_instance", "replace", "custom_scene"), &InstancePlaceholder::create_instance, DEFVAL(false), DEFVAL(Variant()));
	ClassDB::bind_method(D_METHOD("get_instance_path"), &InstancePlaceholder::get_instance_path);
}

InstancePlaceholder::InstancePlaceholder() {
}
