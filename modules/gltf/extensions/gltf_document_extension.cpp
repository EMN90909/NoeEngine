// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "gltf_document_extension.h"

void GLTFDocumentExtension::_bind_methods() {
	// Import process.
	GDVIRTUAL_BIND(_import_preflight, "state", "extensions");
	GDVIRTUAL_BIND(_get_supported_extensions);
	GDVIRTUAL_BIND(_parse_node_extensions, "state", "gltf_node", "extensions");
	GDVIRTUAL_BIND(_generate_scene_node, "state", "gltf_node", "scene_parent");
	GDVIRTUAL_BIND(_import_post_parse, "state");
	GDVIRTUAL_BIND(_import_node, "state", "gltf_node", "json", "node");
	GDVIRTUAL_BIND(_import_post, "state", "root");
	// Export process.
	GDVIRTUAL_BIND(_export_preflight, "state", "root");
	GDVIRTUAL_BIND(_convert_scene_node, "state", "gltf_node", "scene_node");
	GDVIRTUAL_BIND(_export_node, "state", "gltf_node", "json", "node");
	GDVIRTUAL_BIND(_export_post, "state");
}

// Import process.
Error GLTFDocumentExtension::import_preflight(Ref<GLTFState> p_state, Vector<String> p_extensions) {
	ERR_FAIL_NULL_V(p_state, ERR_INVALID_PARAMETER);
	Error err = OK;
	GDVIRTUAL_CALL(_import_preflight, p_state, p_extensions, err);
	return err;
}

Vector<String> GLTFDocumentExtension::get_supported_extensions() {
	Vector<String> ret;
	GDVIRTUAL_CALL(_get_supported_extensions, ret);
	return ret;
}

Error GLTFDocumentExtension::parse_node_extensions(Ref<GLTFState> p_state, Ref<GLTFNode> p_gltf_node, Dictionary &p_extensions) {
	ERR_FAIL_NULL_V(p_state, ERR_INVALID_PARAMETER);
	ERR_FAIL_NULL_V(p_gltf_node, ERR_INVALID_PARAMETER);
	Error err = OK;
	GDVIRTUAL_CALL(_parse_node_extensions, p_state, p_gltf_node, p_extensions, err);
	return err;
}

Node3D *GLTFDocumentExtension::generate_scene_node(Ref<GLTFState> p_state, Ref<GLTFNode> p_gltf_node, Node *p_scene_parent) {
	ERR_FAIL_NULL_V(p_state, nullptr);
	ERR_FAIL_NULL_V(p_gltf_node, nullptr);
	ERR_FAIL_NULL_V(p_scene_parent, nullptr);
	Node3D *ret_node = nullptr;
	GDVIRTUAL_CALL(_generate_scene_node, p_state, p_gltf_node, p_scene_parent, ret_node);
	return ret_node;
}

Error GLTFDocumentExtension::import_post_parse(Ref<GLTFState> p_state) {
	ERR_FAIL_NULL_V(p_state, ERR_INVALID_PARAMETER);
	Error err = OK;
	GDVIRTUAL_CALL(_import_post_parse, p_state, err);
	return err;
}

Error GLTFDocumentExtension::import_node(Ref<GLTFState> p_state, Ref<GLTFNode> p_gltf_node, Dictionary &r_dict, Node *p_node) {
	ERR_FAIL_NULL_V(p_state, ERR_INVALID_PARAMETER);
	ERR_FAIL_NULL_V(p_gltf_node, ERR_INVALID_PARAMETER);
	ERR_FAIL_NULL_V(p_node, ERR_INVALID_PARAMETER);
	Error err = OK;
	GDVIRTUAL_CALL(_import_node, p_state, p_gltf_node, r_dict, p_node, err);
	return err;
}

Error GLTFDocumentExtension::import_post(Ref<GLTFState> p_state, Node *p_root) {
	ERR_FAIL_NULL_V(p_root, ERR_INVALID_PARAMETER);
	ERR_FAIL_NULL_V(p_state, ERR_INVALID_PARAMETER);
	Error err = OK;
	GDVIRTUAL_CALL(_import_post, p_state, p_root, err);
	return err;
}

// Export process.
Error GLTFDocumentExtension::export_preflight(Ref<GLTFState> p_state, Node *p_root) {
	ERR_FAIL_NULL_V(p_root, ERR_INVALID_PARAMETER);
	Error err = OK;
	GDVIRTUAL_CALL(_export_preflight, p_state, p_root, err);
	return err;
}

void GLTFDocumentExtension::convert_scene_node(Ref<GLTFState> p_state, Ref<GLTFNode> p_gltf_node, Node *p_scene_node) {
	ERR_FAIL_NULL(p_state);
	ERR_FAIL_NULL(p_gltf_node);
	ERR_FAIL_NULL(p_scene_node);
	GDVIRTUAL_CALL(_convert_scene_node, p_state, p_gltf_node, p_scene_node);
}

Error GLTFDocumentExtension::export_node(Ref<GLTFState> p_state, Ref<GLTFNode> p_gltf_node, Dictionary &r_dict, Node *p_node) {
	ERR_FAIL_NULL_V(p_state, ERR_INVALID_PARAMETER);
	ERR_FAIL_NULL_V(p_gltf_node, ERR_INVALID_PARAMETER);
	ERR_FAIL_NULL_V(p_node, ERR_INVALID_PARAMETER);
	Error err = OK;
	GDVIRTUAL_CALL(_export_node, p_state, p_gltf_node, r_dict, p_node, err);
	return err;
}

Error GLTFDocumentExtension::export_post(Ref<GLTFState> p_state) {
	ERR_FAIL_NULL_V(p_state, ERR_INVALID_PARAMETER);
	Error err = OK;
	GDVIRTUAL_CALL(_export_post, p_state, err);
	return err;
}
