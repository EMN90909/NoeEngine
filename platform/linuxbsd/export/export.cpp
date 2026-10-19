// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "export.h"

#include "editor/export/editor_export.h"
#include "export_plugin.h"

void register_linuxbsd_exporter() {
	Ref<EditorExportPlatformLinuxBSD> platform;
	platform.instantiate();
	platform->set_name("Linux/X11");
	platform->set_os_name("Linux");
	platform->set_chmod_flags(0755);

	EditorExport::get_singleton()->add_export_platform(platform);
}
