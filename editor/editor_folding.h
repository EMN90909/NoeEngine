// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef EDITOR_FOLDING_H
#define EDITOR_FOLDING_H

#include "scene/main/node.h"

class EditorFolding {
	Vector<String> _get_unfolds(const Object *p_object);
	void _set_unfolds(Object *p_object, const Vector<String> &p_unfolds);

	void _fill_folds(const Node *p_root, const Node *p_node, Array &p_folds, Array &resource_folds, Array &nodes_folded, HashSet<Ref<Resource>> &resources);

	void _do_object_unfolds(Object *p_object, HashSet<Ref<Resource>> &resources);
	void _do_node_unfolds(Node *p_root, Node *p_node, HashSet<Ref<Resource>> &resources);

public:
	void save_resource_folding(const Ref<Resource> &p_resource, const String &p_path);
	void load_resource_folding(Ref<Resource> p_resource, const String &p_path);

	void save_scene_folding(const Node *p_scene, const String &p_path);
	void load_scene_folding(Node *p_scene, const String &p_path);

	void unfold_scene(Node *p_scene);

	bool has_folding_data(const String &p_path);

	EditorFolding();
};

#endif // EDITOR_FOLDING_H
