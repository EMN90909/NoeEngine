// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "export.h"

#include "editor/export/editor_export.h"
#include "export_plugin.h"

void register_ios_exporter() {
	Ref<EditorExportPlatformIOS> platform;
	platform.instantiate();

	EditorExport::get_singleton()->add_export_platform(platform);
}
