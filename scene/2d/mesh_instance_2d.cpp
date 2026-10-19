// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "mesh_instance_2d.h"

#include "scene/scene_string_names.h"

void MeshInstance2D::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_DRAW: {
			if (mesh.is_valid()) {
				draw_mesh(mesh, texture);
			}
		} break;
	}
}

void MeshInstance2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_mesh", "mesh"), &MeshInstance2D::set_mesh);
	ClassDB::bind_method(D_METHOD("get_mesh"), &MeshInstance2D::get_mesh);

	ClassDB::bind_method(D_METHOD("set_texture", "texture"), &MeshInstance2D::set_texture);
	ClassDB::bind_method(D_METHOD("get_texture"), &MeshInstance2D::get_texture);

	ADD_SIGNAL(MethodInfo("texture_changed"));

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "mesh", PROPERTY_HINT_RESOURCE_TYPE, "Mesh"), "set_mesh", "get_mesh");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_texture", "get_texture");
}

void MeshInstance2D::set_mesh(const Ref<Mesh> &p_mesh) {
	mesh = p_mesh;
	queue_redraw();
}

Ref<Mesh> MeshInstance2D::get_mesh() const {
	return mesh;
}

void MeshInstance2D::set_texture(const Ref<Texture2D> &p_texture) {
	if (p_texture == texture) {
		return;
	}
	texture = p_texture;
	queue_redraw();
	emit_signal(SceneStringNames::get_singleton()->texture_changed);
}

Ref<Texture2D> MeshInstance2D::get_texture() const {
	return texture;
}

#ifdef TOOLS_ENABLED
Rect2 MeshInstance2D::_edit_get_rect() const {
	if (mesh.is_valid()) {
		AABB aabb = mesh->get_aabb();
		return Rect2(aabb.position.x, aabb.position.y, aabb.size.x, aabb.size.y);
	}

	return Node2D::_edit_get_rect();
}

bool MeshInstance2D::_edit_use_rect() const {
	return mesh.is_valid();
}
#endif

MeshInstance2D::MeshInstance2D() {
}
