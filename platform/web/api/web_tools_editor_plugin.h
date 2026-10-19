// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef WEB_TOOLS_EDITOR_PLUGIN_H
#define WEB_TOOLS_EDITOR_PLUGIN_H

#if defined(TOOLS_ENABLED) && defined(WEB_ENABLED)
#include "core/io/zip_io.h"
#include "editor/editor_plugin.h"

class WebToolsEditorPlugin : public EditorPlugin {
	GDCLASS(WebToolsEditorPlugin, EditorPlugin);

private:
	void _zip_file(String p_path, String p_base_path, zipFile p_zip);
	void _zip_recursive(String p_path, String p_base_path, zipFile p_zip);
	void _download_zip();

public:
	static void initialize();

	WebToolsEditorPlugin();
};
#else
class WebToolsEditorPlugin {
public:
	static void initialize() {}
};
#endif

#endif // WEB_TOOLS_EDITOR_PLUGIN_H
