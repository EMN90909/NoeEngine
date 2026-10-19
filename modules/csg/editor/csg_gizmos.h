// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef CSG_GIZMOS_H
#define CSG_GIZMOS_H

#ifdef TOOLS_ENABLED

#include "../csg_shape.h"
#include "editor/editor_plugin.h"
#include "editor/plugins/node_3d_editor_gizmos.h"

class CSGShape3DGizmoPlugin : public EditorNode3DGizmoPlugin {
	GDCLASS(CSGShape3DGizmoPlugin, EditorNode3DGizmoPlugin);

public:
	virtual bool has_gizmo(Node3D *p_spatial) override;
	virtual String get_gizmo_name() const override;
	virtual int get_priority() const override;
	virtual bool is_selectable_when_hidden() const override;
	virtual void redraw(EditorNode3DGizmo *p_gizmo) override;

	virtual String get_handle_name(const EditorNode3DGizmo *p_gizmo, int p_id, bool p_secondary) const override;
	virtual Variant get_handle_value(const EditorNode3DGizmo *p_gizmo, int p_id, bool p_secondary) const override;
	virtual void set_handle(const EditorNode3DGizmo *p_gizmo, int p_id, bool p_secondary, Camera3D *p_camera, const Point2 &p_point) override;
	virtual void commit_handle(const EditorNode3DGizmo *p_gizmo, int p_id, bool p_secondary, const Variant &p_restore, bool p_cancel) override;

	CSGShape3DGizmoPlugin();
};

class EditorPluginCSG : public EditorPlugin {
	GDCLASS(EditorPluginCSG, EditorPlugin);

public:
	EditorPluginCSG();
};

#endif // TOOLS_ENABLED

#endif // CSG_GIZMOS_H
