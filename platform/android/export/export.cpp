// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "export.h"

#include "core/os/os.h"
#include "editor/editor_settings.h"
#include "editor/export/editor_export.h"
#include "export_plugin.h"

void register_android_exporter() {
#ifndef ANDROID_ENABLED
	EDITOR_DEF("export/android/android_sdk_path", "");
	EditorSettings::get_singleton()->add_property_hint(PropertyInfo(Variant::STRING, "export/android/android_sdk_path", PROPERTY_HINT_GLOBAL_DIR));
	EDITOR_DEF("export/android/debug_keystore", "");
	EditorSettings::get_singleton()->add_property_hint(PropertyInfo(Variant::STRING, "export/android/debug_keystore", PROPERTY_HINT_GLOBAL_FILE, "*.keystore,*.jks"));
	EDITOR_DEF("export/android/debug_keystore_user", "androiddebugkey");
	EDITOR_DEF("export/android/debug_keystore_pass", "android");
	EDITOR_DEF("export/android/force_system_user", false);

	EDITOR_DEF("export/android/shutdown_adb_on_exit", true);

	EDITOR_DEF("export/android/one_click_deploy_clear_previous_install", false);
#endif

	Ref<EditorExportPlatformAndroid> exporter = Ref<EditorExportPlatformAndroid>(memnew(EditorExportPlatformAndroid));
	EditorExport::get_singleton()->add_export_platform(exporter);
}
