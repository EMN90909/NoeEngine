// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef WORLD_ENVIRONMENT_H
#define WORLD_ENVIRONMENT_H

#include "scene/main/node.h"
#include "scene/resources/camera_attributes.h"
#include "scene/resources/environment.h"

class WorldEnvironment : public Node {
	GDCLASS(WorldEnvironment, Node);

	Ref<Environment> environment;
	Ref<CameraAttributes> camera_attributes;

	void _update_current_environment();
	void _update_current_camera_attributes();

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	void set_environment(const Ref<Environment> &p_environment);
	Ref<Environment> get_environment() const;

	void set_camera_attributes(const Ref<CameraAttributes> &p_camera_attributes);
	Ref<CameraAttributes> get_camera_attributes() const;

	PackedStringArray get_configuration_warnings() const override;

	WorldEnvironment();
};

#endif // WORLD_ENVIRONMENT_H
