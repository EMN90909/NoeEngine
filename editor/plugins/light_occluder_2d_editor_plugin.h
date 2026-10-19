// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef LIGHT_OCCLUDER_2D_EDITOR_PLUGIN_H
#define LIGHT_OCCLUDER_2D_EDITOR_PLUGIN_H

#include "editor/plugins/abstract_polygon_2d_editor.h"
#include "scene/2d/light_occluder_2d.h"

class LightOccluder2DEditor : public AbstractPolygon2DEditor {
	GDCLASS(LightOccluder2DEditor, AbstractPolygon2DEditor);

	LightOccluder2D *node = nullptr;

	Ref<OccluderPolygon2D> _ensure_occluder() const;

protected:
	virtual Node2D *_get_node() const override;
	virtual void _set_node(Node *p_polygon) override;

	virtual bool _is_line() const override;
	virtual int _get_polygon_count() const override;
	virtual Variant _get_polygon(int p_idx) const override;
	virtual void _set_polygon(int p_idx, const Variant &p_polygon) const override;

	virtual void _action_set_polygon(int p_idx, const Variant &p_previous, const Variant &p_polygon) override;

	virtual bool _has_resource() const override;
	virtual void _create_resource() override;

public:
	LightOccluder2DEditor();
};

class LightOccluder2DEditorPlugin : public AbstractPolygon2DEditorPlugin {
	GDCLASS(LightOccluder2DEditorPlugin, AbstractPolygon2DEditorPlugin);

public:
	LightOccluder2DEditorPlugin();
};

#endif // LIGHT_OCCLUDER_2D_EDITOR_PLUGIN_H
