// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef HISTORY_DOCK_H
#define HISTORY_DOCK_H

#include "scene/gui/box_container.h"

class CheckBox;
class ItemList;
class EditorUndoRedoManager;

class HistoryDock : public VBoxContainer {
	GDCLASS(HistoryDock, VBoxContainer);

	EditorUndoRedoManager *ur_manager;
	ItemList *action_list = nullptr;

	CheckBox *current_scene_checkbox = nullptr;
	CheckBox *global_history_checkbox = nullptr;

	bool need_refresh = true;
	int current_version = 0;

	void on_history_changed();
	void refresh_history();
	void on_version_changed();
	void refresh_version();

protected:
	void _notification(int p_notification);

public:
	void seek_history(int p_index);

	HistoryDock();
};

#endif // HISTORY_DOCK_H
