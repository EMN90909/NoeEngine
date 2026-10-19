// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef IMPORT_DEFAULTS_EDITOR_H
#define IMPORT_DEFAULTS_EDITOR_H

#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/option_button.h"

class ImportDefaultsEditorSettings;
class EditorInspector;

class ImportDefaultsEditor : public VBoxContainer {
	GDCLASS(ImportDefaultsEditor, VBoxContainer)

	OptionButton *importers = nullptr;
	Button *save_defaults = nullptr;
	Button *reset_defaults = nullptr;

	EditorInspector *inspector = nullptr;

	ImportDefaultsEditorSettings *settings = nullptr;

	void _update_importer();
	void _importer_selected(int p_index);

	void _reset();
	void _save();

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	void clear();

	ImportDefaultsEditor();
	~ImportDefaultsEditor();
};

#endif // IMPORT_DEFAULTS_EDITOR_H
