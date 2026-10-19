// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef ROOT_MOTION_EDITOR_PLUGIN_H
#define ROOT_MOTION_EDITOR_PLUGIN_H

#include "editor/editor_inspector.h"

class Tree;

class EditorPropertyRootMotion : public EditorProperty {
	GDCLASS(EditorPropertyRootMotion, EditorProperty);
	Button *assign = nullptr;
	Button *clear = nullptr;
	NodePath base_hint;

	ConfirmationDialog *filter_dialog = nullptr;
	Tree *filters = nullptr;

	void _confirmed();
	void _node_assign();
	void _node_clear();

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	virtual void update_property() override;
	void setup(const NodePath &p_base_hint);
	EditorPropertyRootMotion();
};

class EditorInspectorRootMotionPlugin : public EditorInspectorPlugin {
	GDCLASS(EditorInspectorRootMotionPlugin, EditorInspectorPlugin);

public:
	virtual bool can_handle(Object *p_object) override;
	virtual bool parse_property(Object *p_object, const Variant::Type p_type, const String &p_path, const PropertyHint p_hint, const String &p_hint_text, const BitField<PropertyUsageFlags> p_usage, const bool p_wide = false) override;
};

#endif // ROOT_MOTION_EDITOR_PLUGIN_H
