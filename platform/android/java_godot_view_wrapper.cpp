// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "java_godot_view_wrapper.h"

#include "thread_jandroid.h"

GodotJavaViewWrapper::GodotJavaViewWrapper(jobject godot_view) {
	JNIEnv *env = get_jni_env();
	ERR_FAIL_NULL(env);

	_godot_view = env->NewGlobalRef(godot_view);

	_cls = (jclass)env->NewGlobalRef(env->GetObjectClass(godot_view));

	int android_device_api_level = android_get_device_api_level();
	if (android_device_api_level >= __ANDROID_API_N__) {
		_configure_pointer_icon = env->GetMethodID(_cls, "configurePointerIcon", "(ILjava/lang/String;FF)V");
		_set_pointer_icon = env->GetMethodID(_cls, "setPointerIcon", "(I)V");
	}
	if (android_device_api_level >= __ANDROID_API_O__) {
		_request_pointer_capture = env->GetMethodID(_cls, "requestPointerCapture", "()V");
		_release_pointer_capture = env->GetMethodID(_cls, "releasePointerCapture", "()V");
	}

	_can_capture_pointer = env->GetMethodID(_cls, "canCapturePointer", "()Z");
}

bool GodotJavaViewWrapper::can_update_pointer_icon() const {
	return _configure_pointer_icon != nullptr && _set_pointer_icon != nullptr;
}

bool GodotJavaViewWrapper::can_capture_pointer() const {
	// We can capture the pointer if the other jni capture method ids are initialized,
	// and GodotView#canCapturePointer() returns true.
	if (_request_pointer_capture != nullptr && _release_pointer_capture != nullptr && _can_capture_pointer != nullptr) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_NULL_V(env, false);

		return env->CallBooleanMethod(_godot_view, _can_capture_pointer);
	}

	return false;
}

void GodotJavaViewWrapper::request_pointer_capture() {
	if (_request_pointer_capture != nullptr) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_NULL(env);

		env->CallVoidMethod(_godot_view, _request_pointer_capture);
	}
}

void GodotJavaViewWrapper::release_pointer_capture() {
	if (_release_pointer_capture != nullptr) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_NULL(env);

		env->CallVoidMethod(_godot_view, _release_pointer_capture);
	}
}

void GodotJavaViewWrapper::configure_pointer_icon(int pointer_type, const String &image_path, const Vector2 &p_hotspot) {
	if (_configure_pointer_icon != nullptr) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_NULL(env);

		jstring jImagePath = env->NewStringUTF(image_path.utf8().get_data());
		env->CallVoidMethod(_godot_view, _configure_pointer_icon, pointer_type, jImagePath, p_hotspot.x, p_hotspot.y);
	}
}

void GodotJavaViewWrapper::set_pointer_icon(int pointer_type) {
	if (_set_pointer_icon != nullptr) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_NULL(env);

		env->CallVoidMethod(_godot_view, _set_pointer_icon, pointer_type);
	}
}

GodotJavaViewWrapper::~GodotJavaViewWrapper() {
	JNIEnv *env = get_jni_env();
	ERR_FAIL_NULL(env);

	env->DeleteGlobalRef(_godot_view);
	env->DeleteGlobalRef(_cls);
}
