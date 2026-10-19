// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef EDITOR_ABOUT_H
#define EDITOR_ABOUT_H

#include "scene/gui/dialogs.h"
#include "scene/gui/item_list.h"
#include "scene/gui/link_button.h"
#include "scene/gui/rich_text_label.h"
#include "scene/gui/scroll_container.h"
#include "scene/gui/separator.h"
#include "scene/gui/split_container.h"
#include "scene/gui/tab_container.h"
#include "scene/gui/texture_rect.h"
#include "scene/gui/tree.h"

#include "editor/editor_scale.h"

/**
 * NOTE: Do not assume the EditorNode singleton to be available in this class' methods.
 * EditorAbout is also used from the project manager where EditorNode isn't initialized.
 */
class EditorAbout : public AcceptDialog {
	GDCLASS(EditorAbout, AcceptDialog);

private:
	void _license_tree_selected();
	void _version_button_pressed();
	ScrollContainer *_populate_list(const String &p_name, const List<String> &p_sections, const char *const *const p_src[], const int p_flag_single_column = 0);

	LinkButton *version_btn = nullptr;
	Tree *_tpl_tree = nullptr;
	RichTextLabel *_license_text = nullptr;
	RichTextLabel *_tpl_text = nullptr;
	TextureRect *_logo = nullptr;

	void _theme_changed();

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	TextureRect *get_logo() const;

	EditorAbout();
	~EditorAbout();
};

#endif // EDITOR_ABOUT_H
