// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef TEXTURE_3D_EDITOR_PLUGIN_H
#define TEXTURE_3D_EDITOR_PLUGIN_H

#include "editor/editor_inspector.h"
#include "editor/editor_plugin.h"
#include "scene/gui/spin_box.h"
#include "scene/resources/shader.h"
#include "scene/resources/texture.h"

class Texture3DEditor : public Control {
	GDCLASS(Texture3DEditor, Control);

	SpinBox *layer = nullptr;
	Label *info = nullptr;
	Ref<Texture3D> texture;

	Ref<Shader> shader;
	Ref<ShaderMaterial> material;

	Control *texture_rect = nullptr;

	void _make_shaders();

	void _update_material();
	bool setting;
	void _layer_changed(double) {
		if (!setting) {
			_update_material();
		}
	}

	void _texture_rect_update_area();
	void _texture_rect_draw();

	void _texture_changed();

protected:
	void _notification(int p_what);

public:
	void edit(Ref<Texture3D> p_texture);
	Texture3DEditor();
	~Texture3DEditor();
};

class EditorInspectorPlugin3DTexture : public EditorInspectorPlugin {
	GDCLASS(EditorInspectorPlugin3DTexture, EditorInspectorPlugin);

public:
	virtual bool can_handle(Object *p_object) override;
	virtual void parse_begin(Object *p_object) override;
};

class Texture3DEditorPlugin : public EditorPlugin {
	GDCLASS(Texture3DEditorPlugin, EditorPlugin);

public:
	virtual String get_name() const override { return "Texture3D"; }

	Texture3DEditorPlugin();
};

#endif // TEXTURE_3D_EDITOR_PLUGIN_H
