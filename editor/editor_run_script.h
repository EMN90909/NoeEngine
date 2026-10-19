// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef EDITOR_RUN_SCRIPT_H
#define EDITOR_RUN_SCRIPT_H

#include "core/object/ref_counted.h"
#include "editor/editor_plugin.h"

class EditorNode;

class EditorScript : public RefCounted {
	GDCLASS(EditorScript, RefCounted);

	EditorNode *editor = nullptr;

protected:
	static void _bind_methods();
	GDVIRTUAL0(_run)

public:
	void add_root_node(Node *p_node);
	Node *get_scene();
	EditorInterface *get_editor_interface();
	virtual void _run();

	void set_editor(EditorNode *p_editor);
	EditorScript();
};

#endif // EDITOR_RUN_SCRIPT_H
