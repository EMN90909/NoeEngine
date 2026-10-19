// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "bit_map_editor_plugin.h"

#include "editor/editor_scale.h"
#include "scene/gui/label.h"
#include "scene/gui/texture_rect.h"

void BitMapEditor::setup(const Ref<BitMap> &p_bitmap) {
	texture_rect->set_texture(ImageTexture::create_from_image(p_bitmap->convert_to_image()));
	size_label->set_text(vformat(String::utf8("%s×%s"), p_bitmap->get_size().width, p_bitmap->get_size().height));
}

BitMapEditor::BitMapEditor() {
	texture_rect = memnew(TextureRect);
	texture_rect->set_stretch_mode(TextureRect::STRETCH_KEEP_ASPECT_CENTERED);
	texture_rect->set_texture_filter(TEXTURE_FILTER_NEAREST);
	texture_rect->set_custom_minimum_size(Size2(0, 250) * EDSCALE);
	add_child(texture_rect);

	size_label = memnew(Label);
	size_label->set_horizontal_alignment(HORIZONTAL_ALIGNMENT_RIGHT);
	add_child(size_label);

	// Reduce extra padding on top and bottom of size label.
	Ref<StyleBoxEmpty> stylebox;
	stylebox.instantiate();
	stylebox->set_content_margin(SIDE_RIGHT, 4 * EDSCALE);
	size_label->add_theme_style_override("normal", stylebox);
}

///////////////////////

bool EditorInspectorPluginBitMap::can_handle(Object *p_object) {
	return Object::cast_to<BitMap>(p_object) != nullptr;
}

void EditorInspectorPluginBitMap::parse_begin(Object *p_object) {
	BitMap *bitmap = Object::cast_to<BitMap>(p_object);
	if (!bitmap) {
		return;
	}
	Ref<BitMap> bm(bitmap);

	BitMapEditor *editor = memnew(BitMapEditor);
	editor->setup(bm);
	add_custom_control(editor);
}

///////////////////////

BitMapEditorPlugin::BitMapEditorPlugin() {
	Ref<EditorInspectorPluginBitMap> plugin;
	plugin.instantiate();
	add_inspector_plugin(plugin);
}
