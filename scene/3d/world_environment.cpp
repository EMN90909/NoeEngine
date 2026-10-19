// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "world_environment.h"

#include "scene/3d/node_3d.h"
#include "scene/main/window.h"

void WorldEnvironment::_notification(int p_what) {
	switch (p_what) {
		case Node3D::NOTIFICATION_ENTER_WORLD:
		case Node3D::NOTIFICATION_ENTER_TREE: {
			if (environment.is_valid()) {
				add_to_group("_world_environment_" + itos(get_viewport()->find_world_3d()->get_scenario().get_id()));
				_update_current_environment();
			}

			if (camera_attributes.is_valid()) {
				add_to_group("_world_camera_attributes_" + itos(get_viewport()->find_world_3d()->get_scenario().get_id()));
				_update_current_camera_attributes();
			}
		} break;

		case Node3D::NOTIFICATION_EXIT_WORLD:
		case Node3D::NOTIFICATION_EXIT_TREE: {
			if (environment.is_valid()) {
				remove_from_group("_world_environment_" + itos(get_viewport()->find_world_3d()->get_scenario().get_id()));
				_update_current_environment();
			}

			if (camera_attributes.is_valid()) {
				remove_from_group("_world_camera_attributes_" + itos(get_viewport()->find_world_3d()->get_scenario().get_id()));
				_update_current_camera_attributes();
			}
		} break;
	}
}

void WorldEnvironment::_update_current_environment() {
	WorldEnvironment *first = Object::cast_to<WorldEnvironment>(get_tree()->get_first_node_in_group("_world_environment_" + itos(get_viewport()->find_world_3d()->get_scenario().get_id())));

	if (first) {
		get_viewport()->find_world_3d()->set_environment(first->environment);
	} else {
		get_viewport()->find_world_3d()->set_environment(Ref<Environment>());
	}
	get_tree()->call_group_flags(SceneTree::GROUP_CALL_DEFERRED, "_world_environment_" + itos(get_viewport()->find_world_3d()->get_scenario().get_id()), "update_configuration_warnings");
}

void WorldEnvironment::_update_current_camera_attributes() {
	WorldEnvironment *first = Object::cast_to<WorldEnvironment>(get_tree()->get_first_node_in_group("_world_camera_attributes_" + itos(get_viewport()->find_world_3d()->get_scenario().get_id())));
	if (first) {
		get_viewport()->find_world_3d()->set_camera_attributes(first->camera_attributes);
	} else {
		get_viewport()->find_world_3d()->set_camera_attributes(Ref<CameraAttributes>());
	}

	get_tree()->call_group_flags(SceneTree::GROUP_CALL_DEFERRED, "_world_camera_attributes_" + itos(get_viewport()->find_world_3d()->get_scenario().get_id()), "update_configuration_warnings");
}

void WorldEnvironment::set_environment(const Ref<Environment> &p_environment) {
	if (environment == p_environment) {
		return;
	}
	if (is_inside_tree() && environment.is_valid()) {
		remove_from_group("_world_environment_" + itos(get_viewport()->find_world_3d()->get_scenario().get_id()));
	}

	environment = p_environment;

	if (is_inside_tree() && environment.is_valid()) {
		add_to_group("_world_environment_" + itos(get_viewport()->find_world_3d()->get_scenario().get_id()));
	}

	if (is_inside_tree()) {
		_update_current_environment();
	} else {
		update_configuration_warnings();
	}
}

Ref<Environment> WorldEnvironment::get_environment() const {
	return environment;
}

void WorldEnvironment::set_camera_attributes(const Ref<CameraAttributes> &p_camera_attributes) {
	if (camera_attributes == p_camera_attributes) {
		return;
	}

	if (is_inside_tree() && camera_attributes.is_valid() && get_viewport()->find_world_3d()->get_camera_attributes() == camera_attributes) {
		remove_from_group("_world_camera_attributes_" + itos(get_viewport()->find_world_3d()->get_scenario().get_id()));
	}

	camera_attributes = p_camera_attributes;
	if (is_inside_tree() && camera_attributes.is_valid()) {
		add_to_group("_world_camera_attributes_" + itos(get_viewport()->find_world_3d()->get_scenario().get_id()));
	}

	if (is_inside_tree()) {
		_update_current_camera_attributes();
	} else {
		update_configuration_warnings();
	}
}

Ref<CameraAttributes> WorldEnvironment::get_camera_attributes() const {
	return camera_attributes;
}

PackedStringArray WorldEnvironment::get_configuration_warnings() const {
	PackedStringArray warnings = Node::get_configuration_warnings();

	if (!environment.is_valid() && !camera_attributes.is_valid()) {
		warnings.push_back(RTR("To have any visible effect, WorldEnvironment requires its \"Environment\" property to contain an Environment, its \"Camera Attributes\" property to contain a CameraAttributes resource, or both."));
	}

	if (!is_inside_tree()) {
		return warnings;
	}

	if (environment.is_valid() && get_viewport()->find_world_3d()->get_environment() != environment) {
		warnings.push_back(("Only the first Environment has an effect in a scene (or set of instantiated scenes)."));
	}

	if (camera_attributes.is_valid() && get_viewport()->find_world_3d()->get_camera_attributes() != camera_attributes) {
		warnings.push_back(RTR("Only one WorldEnvironment is allowed per scene (or set of instantiated scenes)."));
	}

	return warnings;
}

void WorldEnvironment::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_environment", "env"), &WorldEnvironment::set_environment);
	ClassDB::bind_method(D_METHOD("get_environment"), &WorldEnvironment::get_environment);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "environment", PROPERTY_HINT_RESOURCE_TYPE, "Environment"), "set_environment", "get_environment");

	ClassDB::bind_method(D_METHOD("set_camera_attributes", "camera_attributes"), &WorldEnvironment::set_camera_attributes);
	ClassDB::bind_method(D_METHOD("get_camera_attributes"), &WorldEnvironment::get_camera_attributes);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "camera_attributes", PROPERTY_HINT_RESOURCE_TYPE, "CameraAttributesPractical,CameraAttributesPhysical"), "set_camera_attributes", "get_camera_attributes");
}

WorldEnvironment::WorldEnvironment() {
}
