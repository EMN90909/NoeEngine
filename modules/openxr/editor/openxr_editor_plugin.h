// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef OPENXR_EDITOR_PLUGIN_H
#define OPENXR_EDITOR_PLUGIN_H

#include "editor/editor_plugin.h"
#include "openxr_action_map_editor.h"

class OpenXREditorPlugin : public EditorPlugin {
	GDCLASS(OpenXREditorPlugin, EditorPlugin);

	OpenXRActionMapEditor *action_map_editor = nullptr;

public:
	virtual String get_name() const override { return "OpenXRPlugin"; }
	bool has_main_screen() const override { return false; }
	virtual void edit(Object *p_node) override;
	virtual bool handles(Object *p_node) const override;
	virtual void make_visible(bool p_visible) override;

	OpenXREditorPlugin();
	~OpenXREditorPlugin();
};

#endif // OPENXR_EDITOR_PLUGIN_H
