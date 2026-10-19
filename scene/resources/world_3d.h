// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef WORLD_3D_H
#define WORLD_3D_H

#include "core/io/resource.h"
#include "scene/resources/environment.h"
#include "servers/physics_server_3d.h"
#include "servers/rendering_server.h"

class CameraAttributes;
class Camera3D;
class VisibleOnScreenNotifier3D;
struct SpatialIndexer;

class World3D : public Resource {
	GDCLASS(World3D, Resource);

private:
	RID scenario;
	mutable RID space;
	mutable RID navigation_map;

	Ref<Environment> environment;
	Ref<Environment> fallback_environment;
	Ref<CameraAttributes> camera_attributes;

	HashSet<Camera3D *> cameras;

protected:
	static void _bind_methods();

	friend class Camera3D;

	void _register_camera(Camera3D *p_camera);
	void _remove_camera(Camera3D *p_camera);

public:
	RID get_space() const;
	RID get_navigation_map() const;
	RID get_scenario() const;

	void set_environment(const Ref<Environment> &p_environment);
	Ref<Environment> get_environment() const;

	void set_fallback_environment(const Ref<Environment> &p_environment);
	Ref<Environment> get_fallback_environment() const;

	void set_camera_attributes(const Ref<CameraAttributes> &p_camera_attributes);
	Ref<CameraAttributes> get_camera_attributes() const;

	_FORCE_INLINE_ const HashSet<Camera3D *> &get_cameras() const { return cameras; }

	PhysicsDirectSpaceState3D *get_direct_space_state();

	World3D();
	~World3D();
};

#endif // WORLD_3D_H
