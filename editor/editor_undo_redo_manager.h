// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef EDITOR_UNDO_REDO_MANAGER_H
#define EDITOR_UNDO_REDO_MANAGER_H

#include "core/object/class_db.h"
#include "core/object/object.h"
#include "core/object/undo_redo.h"

class EditorUndoRedoManager : public Object {
	GDCLASS(EditorUndoRedoManager, Object);

	static EditorUndoRedoManager *singleton;

public:
	enum SpecialHistory {
		GLOBAL_HISTORY = 0,
		REMOTE_HISTORY = -9,
		INVALID_HISTORY = -99,
	};

	struct Action {
		int history_id = INVALID_HISTORY;
		double timestamp = 0;
		String action_name;
		UndoRedo::MergeMode merge_mode = UndoRedo::MERGE_DISABLE;
	};

	struct History {
		int id = INVALID_HISTORY;
		UndoRedo *undo_redo = nullptr;
		uint64_t saved_version = 1;
		List<Action> undo_stack;
		List<Action> redo_stack;
	};

private:
	HashMap<int, History> history_map;
	Action pending_action;

	bool is_committing = false;

	History *_get_newest_undo();

protected:
	static void _bind_methods();

public:
	History &get_or_create_history(int p_idx);
	UndoRedo *get_history_undo_redo(int p_idx) const;
	int get_history_id_for_object(Object *p_object) const;
	History &get_history_for_object(Object *p_object);

	void create_action_for_history(const String &p_name, int p_history_id, UndoRedo::MergeMode p_mode = UndoRedo::MERGE_DISABLE);
	void create_action(const String &p_name = "", UndoRedo::MergeMode p_mode = UndoRedo::MERGE_DISABLE, Object *p_custom_context = nullptr);

	void add_do_methodp(Object *p_object, const StringName &p_method, const Variant **p_args, int p_argcount);
	void add_undo_methodp(Object *p_object, const StringName &p_method, const Variant **p_args, int p_argcount);

	template <typename... VarArgs>
	void add_do_method(Object *p_object, const StringName &p_method, VarArgs... p_args) {
		Variant args[sizeof...(p_args) + 1] = { p_args..., Variant() }; // +1 makes sure zero sized arrays are also supported.
		const Variant *argptrs[sizeof...(p_args) + 1];
		for (uint32_t i = 0; i < sizeof...(p_args); i++) {
			argptrs[i] = &args[i];
		}

		add_do_methodp(p_object, p_method, sizeof...(p_args) == 0 ? nullptr : (const Variant **)argptrs, sizeof...(p_args));
	}

	template <typename... VarArgs>
	void add_undo_method(Object *p_object, const StringName &p_method, VarArgs... p_args) {
		Variant args[sizeof...(p_args) + 1] = { p_args..., Variant() }; // +1 makes sure zero sized arrays are also supported.
		const Variant *argptrs[sizeof...(p_args) + 1];
		for (uint32_t i = 0; i < sizeof...(p_args); i++) {
			argptrs[i] = &args[i];
		}

		add_undo_methodp(p_object, p_method, sizeof...(p_args) == 0 ? nullptr : (const Variant **)argptrs, sizeof...(p_args));
	}

	void _add_do_method(const Variant **p_args, int p_argcount, Callable::CallError &r_error);
	void _add_undo_method(const Variant **p_args, int p_argcount, Callable::CallError &r_error);

	void add_do_property(Object *p_object, const StringName &p_property, const Variant &p_value);
	void add_undo_property(Object *p_object, const StringName &p_property, const Variant &p_value);
	void add_do_reference(Object *p_object);
	void add_undo_reference(Object *p_object);

	void commit_action(bool p_execute = true);
	bool is_committing_action() const;

	bool undo();
	bool undo_history(int p_id);
	bool redo();
	bool redo_history(int p_id);
	void clear_history(bool p_increase_version = true, int p_idx = INVALID_HISTORY);

	void set_history_as_saved(int p_idx);
	void set_history_as_unsaved(int p_idx);
	bool is_history_unsaved(int p_idx);
	bool has_undo();
	bool has_redo();

	String get_current_action_name();
	int get_current_action_history_id();

	void discard_history(int p_idx, bool p_erase_from_map = true);

	static EditorUndoRedoManager *get_singleton();
	EditorUndoRedoManager();
	~EditorUndoRedoManager();
};

VARIANT_ENUM_CAST(EditorUndoRedoManager::SpecialHistory);

#endif // EDITOR_UNDO_REDO_MANAGER_H
