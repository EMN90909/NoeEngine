// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef EDITOR_ASSET_INSTALLER_H
#define EDITOR_ASSET_INSTALLER_H

#include "scene/gui/dialogs.h"
#include "scene/gui/tree.h"
class EditorAssetInstaller : public ConfirmationDialog {
	GDCLASS(EditorAssetInstaller, ConfirmationDialog);

	Tree *tree = nullptr;
	Label *asset_contents = nullptr;
	String package_path;
	String asset_name;
	AcceptDialog *error = nullptr;
	HashMap<String, TreeItem *> status_map;
	bool updating = false;
	void _item_edited();
	void _check_propagated_to_item(Object *p_obj, int column);
	virtual void ok_pressed() override;

protected:
	static void _bind_methods();

public:
	void open(const String &p_path, int p_depth = 0);

	void set_asset_name(const String &p_asset_name);
	String get_asset_name() const;

	EditorAssetInstaller();
};

#endif // EDITOR_ASSET_INSTALLER_H
