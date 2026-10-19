// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef NOISE_EDITOR_PLUGIN_H
#define NOISE_EDITOR_PLUGIN_H

#ifdef TOOLS_ENABLED

#include "editor/editor_plugin.h"

class NoiseEditorPlugin : public EditorPlugin {
	GDCLASS(NoiseEditorPlugin, EditorPlugin)

public:
	String get_name() const override;

	NoiseEditorPlugin();
};

#endif // TOOLS_ENABLED

#endif // NOISE_EDITOR_PLUGIN_H
