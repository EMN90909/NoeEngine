// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef GLTF_DOCUMENT_EXTENSION_CONVERT_IMPORTER_MESH_H
#define GLTF_DOCUMENT_EXTENSION_CONVERT_IMPORTER_MESH_H

#include "gltf_document_extension.h"

class GLTFDocumentExtensionConvertImporterMesh : public GLTFDocumentExtension {
	GDCLASS(GLTFDocumentExtensionConvertImporterMesh, GLTFDocumentExtension);

protected:
	static void _bind_methods();

public:
	Error import_post(Ref<GLTFState> p_state, Node *p_root) override;
};

#endif // GLTF_DOCUMENT_EXTENSION_CONVERT_IMPORTER_MESH_H
