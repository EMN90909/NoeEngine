// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "export.h"

#include "editor/editor_settings.h"
#include "export_plugin.h"

void register_uwp_exporter() {
#ifdef WINDOWS_ENABLED
	EDITOR_DEF("export/uwp/signtool", "");
	EditorSettings::get_singleton()->add_property_hint(PropertyInfo(Variant::STRING, "export/uwp/signtool", PROPERTY_HINT_GLOBAL_FILE, "*.exe"));
	EDITOR_DEF("export/uwp/debug_certificate", "");
	EditorSettings::get_singleton()->add_property_hint(PropertyInfo(Variant::STRING, "export/uwp/debug_certificate", PROPERTY_HINT_GLOBAL_FILE, "*.pfx"));
	EDITOR_DEF("export/uwp/debug_password", "");
	EDITOR_DEF("export/uwp/debug_algorithm", 2); // SHA256 is the default
	EditorSettings::get_singleton()->add_property_hint(PropertyInfo(Variant::INT, "export/uwp/debug_algorithm", PROPERTY_HINT_ENUM, "MD5,SHA1,SHA256"));
#endif // WINDOWS_ENABLED

	Ref<EditorExportPlatformUWP> exporter;
	exporter.instantiate();
	EditorExport::get_singleton()->add_export_platform(exporter);
}
