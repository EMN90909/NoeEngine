// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "mesh_storage.h"

using namespace RendererDummy;

MeshStorage *MeshStorage::singleton = nullptr;

MeshStorage::MeshStorage() {
	singleton = this;
}

MeshStorage::~MeshStorage() {
	singleton = nullptr;
}

RID MeshStorage::mesh_allocate() {
	return mesh_owner.allocate_rid();
}

void MeshStorage::mesh_initialize(RID p_rid) {
	mesh_owner.initialize_rid(p_rid, DummyMesh());
}

void MeshStorage::mesh_free(RID p_rid) {
	DummyMesh *mesh = mesh_owner.get_or_null(p_rid);
	ERR_FAIL_COND(!mesh);

	mesh_owner.free(p_rid);
}

void MeshStorage::mesh_clear(RID p_mesh) {
	DummyMesh *m = mesh_owner.get_or_null(p_mesh);
	ERR_FAIL_COND(!m);

	m->surfaces.clear();
}
