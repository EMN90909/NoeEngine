// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef SCENE_REPLICATION_CONFIG_H
#define SCENE_REPLICATION_CONFIG_H

#include "core/io/resource.h"

#include "core/variant/typed_array.h"

class SceneReplicationConfig : public Resource {
	GDCLASS(SceneReplicationConfig, Resource);
	OBJ_SAVE_TYPE(SceneReplicationConfig);
	RES_BASE_EXTENSION("repl");

private:
	struct ReplicationProperty {
		NodePath name;
		bool spawn = true;
		bool sync = true;

		bool operator==(const ReplicationProperty &p_to) {
			return name == p_to.name;
		}

		ReplicationProperty() {}

		ReplicationProperty(const NodePath &p_name) {
			name = p_name;
		}
	};

	List<ReplicationProperty> properties;
	List<NodePath> spawn_props;
	List<NodePath> sync_props;

protected:
	static void _bind_methods();

	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;

public:
	TypedArray<NodePath> get_properties() const;

	void add_property(const NodePath &p_path, int p_index = -1);
	void remove_property(const NodePath &p_path);
	bool has_property(const NodePath &p_path) const;

	int property_get_index(const NodePath &p_path) const;
	bool property_get_spawn(const NodePath &p_path);
	void property_set_spawn(const NodePath &p_path, bool p_enabled);

	bool property_get_sync(const NodePath &p_path);
	void property_set_sync(const NodePath &p_path, bool p_enabled);

	const List<NodePath> &get_spawn_properties() { return spawn_props; }
	const List<NodePath> &get_sync_properties() { return sync_props; }

	SceneReplicationConfig() {}
};

#endif // SCENE_REPLICATION_CONFIG_H
