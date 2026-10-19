// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef EDITOR_SECTIONED_INSPECTOR_H
#define EDITOR_SECTIONED_INSPECTOR_H

#include "editor/editor_inspector.h"
#include "scene/gui/split_container.h"
#include "scene/gui/tree.h"

class SectionedInspectorFilter;

class SectionedInspector : public HSplitContainer {
	GDCLASS(SectionedInspector, HSplitContainer);

	ObjectID obj;

	Tree *sections = nullptr;
	SectionedInspectorFilter *filter = nullptr;

	HashMap<String, TreeItem *> section_map;
	EditorInspector *inspector = nullptr;
	LineEdit *search_box = nullptr;

	String selected_category;

	bool restrict_to_basic = false;

	static void _bind_methods();
	void _section_selected();

	void _search_changed(const String &p_what);

protected:
	void _notification(int p_what);

public:
	void register_search_box(LineEdit *p_box);
	EditorInspector *get_inspector();
	void edit(Object *p_object);
	String get_full_item_path(const String &p_item);

	void set_current_section(const String &p_section);
	String get_current_section() const;

	void set_restrict_to_basic_settings(bool p_restrict);
	void update_category_list();

	SectionedInspector();
	~SectionedInspector();
};

#endif // EDITOR_SECTIONED_INSPECTOR_H
