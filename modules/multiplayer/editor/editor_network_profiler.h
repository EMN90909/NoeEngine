// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef EDITOR_NETWORK_PROFILER_H
#define EDITOR_NETWORK_PROFILER_H

#include "scene/debugger/scene_debugger.h"
#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/label.h"
#include "scene/gui/split_container.h"
#include "scene/gui/tree.h"

#include "../multiplayer_debugger.h"

class EditorNetworkProfiler : public VBoxContainer {
	GDCLASS(EditorNetworkProfiler, VBoxContainer)

public:
	struct NodeInfo {
		ObjectID id;
		String type;
		String path;

		NodeInfo() {}
		NodeInfo(const ObjectID &p_id) {
			id = p_id;
			path = String::num_int64(p_id);
		}
	};

private:
	using RPCNodeInfo = MultiplayerDebugger::RPCNodeInfo;
	using SyncInfo = MultiplayerDebugger::SyncInfo;

	bool dirty = false;
	Timer *refresh_timer = nullptr;
	Button *activate = nullptr;
	Button *clear_button = nullptr;
	Tree *counters_display = nullptr;
	LineEdit *incoming_bandwidth_text = nullptr;
	LineEdit *outgoing_bandwidth_text = nullptr;
	Tree *replication_display = nullptr;

	HashMap<ObjectID, RPCNodeInfo> rpc_data;
	HashMap<ObjectID, SyncInfo> sync_data;
	HashMap<ObjectID, NodeInfo> node_data;
	HashSet<ObjectID> missing_node_data;
	Ref<Texture2D> node_icon;

	void _activate_pressed();
	void _clear_pressed();
	void _refresh();
	void _replication_button_clicked(TreeItem *p_item, int p_column, int p_idx, MouseButton p_button);

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	void refresh_rpc_data();
	void refresh_replication_data();

	Array pop_missing_node_data();
	void add_node_data(const NodeInfo &p_info);
	void add_rpc_frame_data(const RPCNodeInfo &p_frame);
	void add_sync_frame_data(const SyncInfo &p_frame);
	void set_bandwidth(int p_incoming, int p_outgoing);
	bool is_profiling();

	EditorNetworkProfiler();
};

#endif // EDITOR_NETWORK_PROFILER_H
