// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef GLTF_DOCUMENT_EXTENSION_H
#define GLTF_DOCUMENT_EXTENSION_H

#include "../gltf_state.h"

class GLTFDocumentExtension : public Resource {
	GDCLASS(GLTFDocumentExtension, Resource);

protected:
	static void _bind_methods();

public:
	// Import process.
	virtual Error import_preflight(Ref<GLTFState> p_state, Vector<String> p_extensions);
	virtual Vector<String> get_supported_extensions();
	virtual Error parse_node_extensions(Ref<GLTFState> p_state, Ref<GLTFNode> p_gltf_node, Dictionary &p_extensions);
	virtual Node3D *generate_scene_node(Ref<GLTFState> p_state, Ref<GLTFNode> p_gltf_node, Node *p_scene_parent);
	virtual Error import_post_parse(Ref<GLTFState> p_state);
	virtual Error import_node(Ref<GLTFState> p_state, Ref<GLTFNode> p_gltf_node, Dictionary &r_json, Node *p_node);
	virtual Error import_post(Ref<GLTFState> p_state, Node *p_node);
	// Export process.
	virtual Error export_preflight(Ref<GLTFState> p_state, Node *p_root);
	virtual void convert_scene_node(Ref<GLTFState> p_state, Ref<GLTFNode> p_gltf_node, Node *p_scene_node);
	virtual Error export_node(Ref<GLTFState> p_state, Ref<GLTFNode> p_gltf_node, Dictionary &r_json, Node *p_node);
	virtual Error export_post(Ref<GLTFState> p_state);

	// Import process.
	GDVIRTUAL2R(Error, _import_preflight, Ref<GLTFState>, Vector<String>);
	GDVIRTUAL0R(Vector<String>, _get_supported_extensions);
	GDVIRTUAL3R(Error, _parse_node_extensions, Ref<GLTFState>, Ref<GLTFNode>, Dictionary);
	GDVIRTUAL3R(Node3D *, _generate_scene_node, Ref<GLTFState>, Ref<GLTFNode>, Node *);
	GDVIRTUAL1R(Error, _import_post_parse, Ref<GLTFState>);
	GDVIRTUAL4R(Error, _import_node, Ref<GLTFState>, Ref<GLTFNode>, Dictionary, Node *);
	GDVIRTUAL2R(Error, _import_post, Ref<GLTFState>, Node *);
	// Export process.
	GDVIRTUAL2R(Error, _export_preflight, Ref<GLTFState>, Node *);
	GDVIRTUAL3(_convert_scene_node, Ref<GLTFState>, Ref<GLTFNode>, Node *);
	GDVIRTUAL4R(Error, _export_node, Ref<GLTFState>, Ref<GLTFNode>, Dictionary, Node *);
	GDVIRTUAL1R(Error, _export_post, Ref<GLTFState>);
};

#endif // GLTF_DOCUMENT_EXTENSION_H
