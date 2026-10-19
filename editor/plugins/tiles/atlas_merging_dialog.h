// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef ATLAS_MERGING_DIALOG_H
#define ATLAS_MERGING_DIALOG_H

#include "editor/editor_properties.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/item_list.h"
#include "scene/gui/texture_rect.h"
#include "scene/resources/tile_set.h"

class EditorFileDialog;

class AtlasMergingDialog : public ConfirmationDialog {
	GDCLASS(AtlasMergingDialog, ConfirmationDialog);

private:
	int commited_actions_count = 0;
	bool delete_original_atlases = true;
	Ref<TileSetAtlasSource> merged;
	LocalVector<HashMap<Vector2i, Vector2i>> merged_mapping;
	Ref<TileSet> tile_set;

	// Settings.
	int next_line_after_column = 30;

	// GUI.
	ItemList *atlas_merging_atlases_list = nullptr;
	EditorPropertyVector2i *texture_region_size_editor_property = nullptr;
	EditorPropertyInteger *columns_editor_property = nullptr;
	TextureRect *preview = nullptr;
	Label *select_2_atlases_label = nullptr;
	EditorFileDialog *editor_file_dialog = nullptr;
	Button *merge_button = nullptr;

	void _property_changed(const StringName &p_property, const Variant &p_value, const String &p_field, bool p_changing);

	void _generate_merged(Vector<Ref<TileSetAtlasSource>> p_atlas_sources, int p_max_columns);
	void _update_texture();
	void _merge_confirmed(String p_path);

protected:
	virtual void ok_pressed() override;
	virtual void cancel_pressed() override;
	virtual void custom_action(const String &) override;

	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;

	void _notification(int p_what);

public:
	void update_tile_set(Ref<TileSet> p_tile_set);

	AtlasMergingDialog();
};

#endif // ATLAS_MERGING_DIALOG_H
