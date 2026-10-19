// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef OPENXR_ACTION_SET_H
#define OPENXR_ACTION_SET_H

#include "core/io/resource.h"

#include "openxr_action.h"

class OpenXRActionSet : public Resource {
	GDCLASS(OpenXRActionSet, Resource);

private:
	String localized_name;
	int priority = 0;

	Array actions;
	void clear_actions();

protected:
	static void _bind_methods();

public:
	static Ref<OpenXRActionSet> new_action_set(const char *p_name, const char *p_localized_name, const int p_priority = 0); // Helper function for adding and setting up an action set

	void set_localized_name(const String p_localized_name); // Set the localized name of this action set
	String get_localized_name() const; // Get the localized name of this action set

	void set_priority(const int p_priority); // Set the priority of this action set
	int get_priority() const; // Get the priority of this action set

	int get_action_count() const; // Retrieve the number of actions in our action set
	void set_actions(Array p_actions); // Set our actions using an array of actions (for loading a resource)
	Array get_actions() const; // Get our actions as an array (for saving a resource)

	Ref<OpenXRAction> get_action(const String p_name) const; // Retrieve an action by name
	void add_action(Ref<OpenXRAction> p_action); // Add a new action to our action set
	void remove_action(Ref<OpenXRAction> p_action); // remove a action from our action set

	Ref<OpenXRAction> add_new_action(const char *p_name, const char *p_localized_name, const OpenXRAction::ActionType p_action_type, const char *p_toplevel_paths); // Helper function for adding and setting up an action

	// TODO add validation to display in the interface that checks if we have duplicate action names within our action set

	~OpenXRActionSet();
};

#endif // OPENXR_ACTION_SET_H
