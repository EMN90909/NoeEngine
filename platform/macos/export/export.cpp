// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "export.h"

#include "export_plugin.h"

void register_macos_exporter() {
#ifndef ANDROID_ENABLED
	EDITOR_DEF("export/macos/rcodesign", "");
#ifdef WINDOWS_ENABLED
	EditorSettings::get_singleton()->add_property_hint(PropertyInfo(Variant::STRING, "export/macos/rcodesign", PROPERTY_HINT_GLOBAL_FILE, "*.exe"));
#else
	EditorSettings::get_singleton()->add_property_hint(PropertyInfo(Variant::STRING, "export/macos/rcodesign", PROPERTY_HINT_GLOBAL_FILE));
#endif
#endif

	Ref<EditorExportPlatformMacOS> platform;
	platform.instantiate();

	EditorExport::get_singleton()->add_export_platform(platform);
}
