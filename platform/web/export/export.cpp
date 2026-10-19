// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "export.h"

#include "editor/editor_settings.h"
#include "export_plugin.h"

void register_web_exporter() {
#ifndef ANDROID_ENABLED
	EDITOR_DEF("export/web/http_host", "localhost");
	EDITOR_DEF("export/web/http_port", 8060);
	EDITOR_DEF("export/web/use_tls", false);
	EDITOR_DEF("export/web/tls_key", "");
	EDITOR_DEF("export/web/tls_certificate", "");
	EditorSettings::get_singleton()->add_property_hint(PropertyInfo(Variant::INT, "export/web/http_port", PROPERTY_HINT_RANGE, "1,65535,1"));
	EditorSettings::get_singleton()->add_property_hint(PropertyInfo(Variant::STRING, "export/web/tls_key", PROPERTY_HINT_GLOBAL_FILE, "*.key"));
	EditorSettings::get_singleton()->add_property_hint(PropertyInfo(Variant::STRING, "export/web/tls_certificate", PROPERTY_HINT_GLOBAL_FILE, "*.crt,*.pem"));
#endif

	Ref<EditorExportPlatformWeb> platform;
	platform.instantiate();
	EditorExport::get_singleton()->add_export_platform(platform);
}
