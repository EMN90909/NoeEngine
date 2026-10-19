// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef EDITOR_NATIVE_SHADER_SOURCE_VISUALIZER_H
#define EDITOR_NATIVE_SHADER_SOURCE_VISUALIZER_H

#include "scene/gui/dialogs.h"
#include "scene/gui/tab_container.h"

class EditorNativeShaderSourceVisualizer : public AcceptDialog {
	GDCLASS(EditorNativeShaderSourceVisualizer, AcceptDialog)
	TabContainer *versions = nullptr;

	void _inspect_shader(RID p_shader);

protected:
	static void _bind_methods();

public:
	EditorNativeShaderSourceVisualizer();
};

#endif // EDITOR_NATIVE_SHADER_SOURCE_VISUALIZER_H
