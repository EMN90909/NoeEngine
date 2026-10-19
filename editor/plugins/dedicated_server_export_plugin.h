// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef DEDICATED_SERVER_EXPORT_PLUGIN_H
#define DEDICATED_SERVER_EXPORT_PLUGIN_H

#include "editor/export/editor_export.h"

class DedicatedServerExportPlugin : public EditorExportPlugin {
private:
	EditorExportPreset::FileExportMode current_export_mode;

	EditorExportPreset::FileExportMode _get_export_mode_for_path(const String &p_path);

protected:
	String _get_name() const override { return "DedicatedServer"; }

	PackedStringArray _get_export_features(const Ref<EditorExportPlatform> &p_platform, bool p_debug) const override;
	uint64_t _get_customization_configuration_hash() const override;

	bool _begin_customize_scenes(const Ref<EditorExportPlatform> &p_platform, const Vector<String> &p_features) override;
	bool _begin_customize_resources(const Ref<EditorExportPlatform> &p_platform, const Vector<String> &p_features) override;

	Node *_customize_scene(Node *p_root, const String &p_path) override;
	Ref<Resource> _customize_resource(const Ref<Resource> &p_resource, const String &p_path) override;

	void _end_customize_scenes() override;
	void _end_customize_resources() override;
};

#endif // DEDICATED_SERVER_EXPORT_PLUGIN_H
