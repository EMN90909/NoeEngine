// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef SHADER_GLOBALS_EDITOR_H
#define SHADER_GLOBALS_EDITOR_H

#include "editor/editor_autoload_settings.h"
#include "editor/editor_data.h"
#include "editor/editor_plugin_settings.h"
#include "editor/editor_sectioned_inspector.h"
#include "scene/gui/tab_container.h"

class ShaderGlobalsEditorInterface;

class ShaderGlobalsEditor : public VBoxContainer {
	GDCLASS(ShaderGlobalsEditor, VBoxContainer)

	ShaderGlobalsEditorInterface *interface = nullptr;
	EditorInspector *inspector = nullptr;

	LineEdit *variable_name = nullptr;
	OptionButton *variable_type = nullptr;
	Button *variable_add = nullptr;

	void _variable_added();
	void _variable_deleted(const String &p_variable);
	void _changed();

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	ShaderGlobalsEditor();
	~ShaderGlobalsEditor();
};

#endif // SHADER_GLOBALS_EDITOR_H
