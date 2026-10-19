// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef RESOURCE_PRELOADER_EDITOR_PLUGIN_H
#define RESOURCE_PRELOADER_EDITOR_PLUGIN_H

#include "editor/editor_plugin.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/panel_container.h"
#include "scene/gui/tree.h"
#include "scene/main/resource_preloader.h"

class EditorFileDialog;

class ResourcePreloaderEditor : public PanelContainer {
	GDCLASS(ResourcePreloaderEditor, PanelContainer);

	enum {
		BUTTON_OPEN_SCENE,
		BUTTON_EDIT_RESOURCE,
		BUTTON_REMOVE
	};

	Button *load = nullptr;
	Button *paste = nullptr;
	Tree *tree = nullptr;
	bool loading_scene;

	EditorFileDialog *file = nullptr;

	AcceptDialog *dialog = nullptr;

	ResourcePreloader *preloader = nullptr;

	void _load_pressed();
	void _files_load_request(const Vector<String> &p_paths);
	void _paste_pressed();
	void _remove_resource(const String &p_to_remove);
	void _update_library();
	void _cell_button_pressed(Object *p_item, int p_column, int p_id, MouseButton p_button);
	void _item_edited();

	Variant get_drag_data_fw(const Point2 &p_point, Control *p_from);
	bool can_drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from) const;
	void drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from);

protected:
	void _notification(int p_what);

	static void _bind_methods();

public:
	void edit(ResourcePreloader *p_preloader);
	ResourcePreloaderEditor();
};

class ResourcePreloaderEditorPlugin : public EditorPlugin {
	GDCLASS(ResourcePreloaderEditorPlugin, EditorPlugin);

	ResourcePreloaderEditor *preloader_editor = nullptr;
	Button *button = nullptr;

public:
	virtual String get_name() const override { return "ResourcePreloader"; }
	bool has_main_screen() const override { return false; }
	virtual void edit(Object *p_object) override;
	virtual bool handles(Object *p_object) const override;
	virtual void make_visible(bool p_visible) override;

	ResourcePreloaderEditorPlugin();
	~ResourcePreloaderEditorPlugin();
};

#endif // RESOURCE_PRELOADER_EDITOR_PLUGIN_H
