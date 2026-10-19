// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef NAVIGATION_POLYGON_EDITOR_PLUGIN_H
#define NAVIGATION_POLYGON_EDITOR_PLUGIN_H

#include "editor/plugins/abstract_polygon_2d_editor.h"
#include "scene/2d/navigation_region_2d.h"

class NavigationPolygonEditor : public AbstractPolygon2DEditor {
	GDCLASS(NavigationPolygonEditor, AbstractPolygon2DEditor);

	NavigationRegion2D *node = nullptr;

	Ref<NavigationPolygon> _ensure_navpoly() const;

protected:
	virtual Node2D *_get_node() const override;
	virtual void _set_node(Node *p_polygon) override;

	virtual int _get_polygon_count() const override;
	virtual Variant _get_polygon(int p_idx) const override;
	virtual void _set_polygon(int p_idx, const Variant &p_polygon) const override;

	virtual void _action_add_polygon(const Variant &p_polygon) override;
	virtual void _action_remove_polygon(int p_idx) override;
	virtual void _action_set_polygon(int p_idx, const Variant &p_previous, const Variant &p_polygon) override;

	virtual bool _has_resource() const override;
	virtual void _create_resource() override;

public:
	NavigationPolygonEditor();
};

class NavigationPolygonEditorPlugin : public AbstractPolygon2DEditorPlugin {
	GDCLASS(NavigationPolygonEditorPlugin, AbstractPolygon2DEditorPlugin);

public:
	NavigationPolygonEditorPlugin();
};

#endif // NAVIGATION_POLYGON_EDITOR_PLUGIN_H
