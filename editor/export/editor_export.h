// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef EDITOR_EXPORT_H
#define EDITOR_EXPORT_H

#include "editor_export_platform.h"
#include "editor_export_plugin.h"

class EditorExport : public Node {
	GDCLASS(EditorExport, Node);

	Vector<Ref<EditorExportPlatform>> export_platforms;
	Vector<Ref<EditorExportPreset>> export_presets;
	Vector<Ref<EditorExportPlugin>> export_plugins;

	StringName _export_presets_updated;

	Timer *save_timer = nullptr;
	bool block_save = false;

	static EditorExport *singleton;

	void _save();

protected:
	friend class EditorExportPreset;
	void save_presets();

	void _notification(int p_what);
	static void _bind_methods();

public:
	static EditorExport *get_singleton() { return singleton; }

	void add_export_platform(const Ref<EditorExportPlatform> &p_platform);
	int get_export_platform_count();
	Ref<EditorExportPlatform> get_export_platform(int p_idx);

	void add_export_preset(const Ref<EditorExportPreset> &p_preset, int p_at_pos = -1);
	int get_export_preset_count() const;
	Ref<EditorExportPreset> get_export_preset(int p_idx);
	void remove_export_preset(int p_idx);

	void add_export_plugin(const Ref<EditorExportPlugin> &p_plugin);
	void remove_export_plugin(const Ref<EditorExportPlugin> &p_plugin);
	Vector<Ref<EditorExportPlugin>> get_export_plugins();

	void load_config();
	void update_export_presets();
	bool poll_export_platforms();

	EditorExport();
	~EditorExport();
};

#endif // EDITOR_EXPORT_H
