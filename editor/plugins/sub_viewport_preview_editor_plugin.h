// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef SUB_VIEWPORT_PREVIEW_EDITOR_PLUGIN_H
#define SUB_VIEWPORT_PREVIEW_EDITOR_PLUGIN_H

#include "editor/editor_plugin.h"
#include "editor/plugins/texture_editor_plugin.h"
#include "scene/main/viewport.h"

class EditorInspectorPluginSubViewportPreview : public EditorInspectorPluginTexture {
	GDCLASS(EditorInspectorPluginSubViewportPreview, EditorInspectorPluginTexture);

public:
	virtual bool can_handle(Object *p_object) override;
	virtual void parse_begin(Object *p_object) override;
};

class SubViewportPreviewEditorPlugin : public EditorPlugin {
	GDCLASS(SubViewportPreviewEditorPlugin, EditorPlugin);

public:
	virtual String get_name() const override { return "SubViewportPreview"; }

	SubViewportPreviewEditorPlugin();
};

#endif // SUB_VIEWPORT_PREVIEW_EDITOR_PLUGIN_H
