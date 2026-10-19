// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef UNDO_REDO_H
#define UNDO_REDO_H

#include "core/object/class_db.h"
#include "core/object/ref_counted.h"

class UndoRedo : public Object {
	GDCLASS(UndoRedo, Object);
	OBJ_SAVE_TYPE(UndoRedo);

public:
	enum MergeMode {
		MERGE_DISABLE,
		MERGE_ENDS,
		MERGE_ALL
	};

	typedef void (*CommitNotifyCallback)(void *p_ud, const String &p_name);

	typedef void (*MethodNotifyCallback)(void *p_ud, Object *p_base, const StringName &p_name, const Variant **p_args, int p_argcount);
	typedef void (*PropertyNotifyCallback)(void *p_ud, Object *p_base, const StringName &p_property, const Variant &p_value);

private:
	struct Operation {
		enum Type {
			TYPE_METHOD,
			TYPE_PROPERTY,
			TYPE_REFERENCE
		} type;

		bool force_keep_in_merge_ends;
		Ref<RefCounted> ref;
		ObjectID object;
		StringName name;
		Callable callable;
		Variant value;

		void delete_reference();
	};

	struct Action {
		String name;
		List<Operation> do_ops;
		List<Operation> undo_ops;
		uint64_t last_tick;
	};

	Vector<Action> actions;
	int current_action = -1;
	bool force_keep_in_merge_ends = false;
	int action_level = 0;
	MergeMode merge_mode = MERGE_DISABLE;
	bool merging = false;
	uint64_t version = 1;

	void _pop_history_tail();
	void _process_operation_list(List<Operation>::Element *E);
	void _discard_redo();
	bool _redo(bool p_execute);

	CommitNotifyCallback callback = nullptr;
	void *callback_ud = nullptr;
	void *method_callback_ud = nullptr;
	void *prop_callback_ud = nullptr;

	MethodNotifyCallback method_callback = nullptr;
	PropertyNotifyCallback property_callback = nullptr;

	int committing = 0;

protected:
	static void _bind_methods();

public:
	void create_action(const String &p_name = "", MergeMode p_mode = MERGE_DISABLE);

	void add_do_method(const Callable &p_callable);
	void add_undo_method(const Callable &p_callable);
	void add_do_property(Object *p_object, const StringName &p_property, const Variant &p_value);
	void add_undo_property(Object *p_object, const StringName &p_property, const Variant &p_value);
	void add_do_reference(Object *p_object);
	void add_undo_reference(Object *p_object);

	void start_force_keep_in_merge_ends();
	void end_force_keep_in_merge_ends();

	bool is_committing_action() const;
	void commit_action(bool p_execute = true);

	bool redo();
	bool undo();
	String get_current_action_name() const;
	int get_action_level() const;

	int get_history_count();
	int get_current_action();
	String get_action_name(int p_id);
	void clear_history(bool p_increase_version = true);

	bool has_undo() const;
	bool has_redo() const;

	uint64_t get_version() const;

	void set_commit_notify_callback(CommitNotifyCallback p_callback, void *p_ud);

	void set_method_notify_callback(MethodNotifyCallback p_method_callback, void *p_ud);
	void set_property_notify_callback(PropertyNotifyCallback p_property_callback, void *p_ud);

	UndoRedo() {}
	~UndoRedo();
};

VARIANT_ENUM_CAST(UndoRedo::MergeMode);

#endif // UNDO_REDO_H
