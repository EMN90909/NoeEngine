// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef JAVA_GODOT_VIEW_WRAPPER_H
#define JAVA_GODOT_VIEW_WRAPPER_H

#include "core/math/vector2.h"
#include <android/log.h>
#include <jni.h>

#include "string_android.h"

// Class that makes functions in java/src/org/godotengine/godot/GodotView.java callable from C++
class GodotJavaViewWrapper {
private:
	jclass _cls;

	jobject _godot_view;

	jmethodID _can_capture_pointer = 0;
	jmethodID _request_pointer_capture = 0;
	jmethodID _release_pointer_capture = 0;

	jmethodID _configure_pointer_icon = 0;
	jmethodID _set_pointer_icon = 0;

public:
	GodotJavaViewWrapper(jobject godot_view);

	bool can_update_pointer_icon() const;
	bool can_capture_pointer() const;

	void request_pointer_capture();
	void release_pointer_capture();

	void configure_pointer_icon(int pointer_type, const String &image_path, const Vector2 &p_hotspot);
	void set_pointer_icon(int pointer_type);

	~GodotJavaViewWrapper();
};

#endif // JAVA_GODOT_VIEW_WRAPPER_H
