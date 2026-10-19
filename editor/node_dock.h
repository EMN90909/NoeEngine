// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef NODE_DOCK_H
#define NODE_DOCK_H

#include "groups_editor.h"

class ConnectionsDock;

class NodeDock : public VBoxContainer {
	GDCLASS(NodeDock, VBoxContainer);

	Button *connections_button = nullptr;
	Button *groups_button = nullptr;

	ConnectionsDock *connections = nullptr;
	GroupsEditor *groups = nullptr;

	HBoxContainer *mode_hb = nullptr;

	Label *select_a_node = nullptr;

private:
	static NodeDock *singleton;

public:
	static NodeDock *get_singleton() { return singleton; }

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	void set_node(Node *p_node);

	void show_groups();
	void show_connections();

	void update_lists();

	NodeDock();
	~NodeDock();
};

#endif // NODE_DOCK_H
