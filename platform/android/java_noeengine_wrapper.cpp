/**************************************************************************/
/*  java_noeengine_wrapper.cpp                                                */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             NOEENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "java_noeengine_wrapper.h"

// JNIEnv is only valid within the thread it belongs to, in a multi threading environment
// we can't cache it.
// For NoeEngine we call most access methods from our thread and we thus get a valid JNIEnv
// from get_jni_env(). For one or two we expect to pass the environment

// TODO we could probably create a base class for this...

NoeEngineJavaWrapper::NoeEngineJavaWrapper(JNIEnv *p_env, jobject p_activity, jobject p_noeengine_instance) {
	noeengine_instance = p_env->NewGlobalRef(p_noeengine_instance);
	activity = p_env->NewGlobalRef(p_activity);

	// get info about our NoeEngine class so we can get pointers and stuff...
	noeengine_class = p_env->FindClass("org/noeengine/noeengine/NoeEngine");
	if (noeengine_class) {
		noeengine_class = (jclass)p_env->NewGlobalRef(noeengine_class);
	} else {
		// this is a pretty serious fail.. bail... pointers will stay 0
		return;
	}
	activity_class = p_env->FindClass("android/app/Activity");
	if (activity_class) {
		activity_class = (jclass)p_env->NewGlobalRef(activity_class);
	} else {
		// this is a pretty serious fail.. bail... pointers will stay 0
		return;
	}

	// get some NoeEngine method pointers...
	_on_video_init = p_env->GetMethodID(noeengine_class, "onVideoInit", "()Z");
	_restart = p_env->GetMethodID(noeengine_class, "restart", "()V");
	_finish = p_env->GetMethodID(noeengine_class, "forceQuit", "(I)Z");
	_set_keep_screen_on = p_env->GetMethodID(noeengine_class, "setKeepScreenOn", "(Z)V");
	_alert = p_env->GetMethodID(noeengine_class, "alert", "(Ljava/lang/String;Ljava/lang/String;)V");
	_get_GLES_version_code = p_env->GetMethodID(noeengine_class, "getGLESVersionCode", "()I");
	_get_clipboard = p_env->GetMethodID(noeengine_class, "getClipboard", "()Ljava/lang/String;");
	_set_clipboard = p_env->GetMethodID(noeengine_class, "setClipboard", "(Ljava/lang/String;)V");
	_has_clipboard = p_env->GetMethodID(noeengine_class, "hasClipboard", "()Z");
	_request_permission = p_env->GetMethodID(noeengine_class, "requestPermission", "(Ljava/lang/String;)Z");
	_request_permissions = p_env->GetMethodID(noeengine_class, "requestPermissions", "()Z");
	_get_granted_permissions = p_env->GetMethodID(noeengine_class, "getGrantedPermissions", "()[Ljava/lang/String;");
	_init_input_devices = p_env->GetMethodID(noeengine_class, "initInputDevices", "()V");
	_get_surface = p_env->GetMethodID(noeengine_class, "getSurface", "()Landroid/view/Surface;");
	_is_activity_resumed = p_env->GetMethodID(noeengine_class, "isActivityResumed", "()Z");
	_vibrate = p_env->GetMethodID(noeengine_class, "vibrate", "(I)V");
	_get_input_fallback_mapping = p_env->GetMethodID(noeengine_class, "getInputFallbackMapping", "()Ljava/lang/String;");
	_on_noeengine_setup_completed = p_env->GetMethodID(noeengine_class, "onNoeEngineSetupCompleted", "()V");
	_on_noeengine_main_loop_started = p_env->GetMethodID(noeengine_class, "onNoeEngineMainLoopStarted", "()V");
	_create_new_noeengine_instance = p_env->GetMethodID(noeengine_class, "createNewNoeEngineInstance", "([Ljava/lang/String;)I");
	_get_render_view = p_env->GetMethodID(noeengine_class, "getRenderView", "()Lorg/noeengine/noeengine/NoeEngineRenderView;");

	// get some Activity method pointers...
	_get_class_loader = p_env->GetMethodID(activity_class, "getClassLoader", "()Ljava/lang/ClassLoader;");
}

NoeEngineJavaWrapper::~NoeEngineJavaWrapper() {
	if (noeengine_view) {
		delete noeengine_view;
	}

	JNIEnv *env = get_jni_env();
	ERR_FAIL_NULL(env);
	env->DeleteGlobalRef(noeengine_instance);
	env->DeleteGlobalRef(noeengine_class);
	env->DeleteGlobalRef(activity);
	env->DeleteGlobalRef(activity_class);
}

jobject NoeEngineJavaWrapper::get_activity() {
	return activity;
}

jobject NoeEngineJavaWrapper::get_member_object(const char *p_name, const char *p_class, JNIEnv *p_env) {
	if (noeengine_class) {
		if (p_env == nullptr) {
			p_env = get_jni_env();
		}
		ERR_FAIL_NULL_V(p_env, nullptr);
		jfieldID fid = p_env->GetStaticFieldID(noeengine_class, p_name, p_class);
		return p_env->GetStaticObjectField(noeengine_class, fid);
	} else {
		return nullptr;
	}
}

jobject NoeEngineJavaWrapper::get_class_loader() {
	if (_get_class_loader) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_NULL_V(env, nullptr);
		return env->CallObjectMethod(activity, _get_class_loader);
	} else {
		return nullptr;
	}
}

NoeEngineJavaViewWrapper *NoeEngineJavaWrapper::get_noeengine_view() {
	if (noeengine_view != nullptr) {
		return noeengine_view;
	}
	if (_get_render_view) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_NULL_V(env, nullptr);
		jobject noeengine_render_view = env->CallObjectMethod(noeengine_instance, _get_render_view);
		if (!env->IsSameObject(noeengine_render_view, nullptr)) {
			noeengine_view = new NoeEngineJavaViewWrapper(noeengine_render_view);
		}
	}
	return noeengine_view;
}

bool NoeEngineJavaWrapper::on_video_init(JNIEnv *p_env) {
	if (_on_video_init) {
		if (p_env == nullptr) {
			p_env = get_jni_env();
		}
		ERR_FAIL_NULL_V(p_env, false);
		return p_env->CallBooleanMethod(noeengine_instance, _on_video_init);
	}
	return false;
}

void NoeEngineJavaWrapper::on_noeengine_setup_completed(JNIEnv *p_env) {
	if (_on_noeengine_setup_completed) {
		if (p_env == nullptr) {
			p_env = get_jni_env();
		}
		p_env->CallVoidMethod(noeengine_instance, _on_noeengine_setup_completed);
	}
}

void NoeEngineJavaWrapper::on_noeengine_main_loop_started(JNIEnv *p_env) {
	if (_on_noeengine_main_loop_started) {
		if (p_env == nullptr) {
			p_env = get_jni_env();
		}
		ERR_FAIL_NULL(p_env);
		p_env->CallVoidMethod(noeengine_instance, _on_noeengine_main_loop_started);
	}
}

void NoeEngineJavaWrapper::restart(JNIEnv *p_env) {
	if (_restart) {
		if (p_env == nullptr) {
			p_env = get_jni_env();
		}
		ERR_FAIL_NULL(p_env);
		p_env->CallVoidMethod(noeengine_instance, _restart);
	}
}

bool NoeEngineJavaWrapper::force_quit(JNIEnv *p_env, int p_instance_id) {
	if (_finish) {
		if (p_env == nullptr) {
			p_env = get_jni_env();
		}
		ERR_FAIL_NULL_V(p_env, false);
		return p_env->CallBooleanMethod(noeengine_instance, _finish, p_instance_id);
	}
	return false;
}

void NoeEngineJavaWrapper::set_keep_screen_on(bool p_enabled) {
	if (_set_keep_screen_on) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_NULL(env);
		env->CallVoidMethod(noeengine_instance, _set_keep_screen_on, p_enabled);
	}
}

void NoeEngineJavaWrapper::alert(const String &p_message, const String &p_title) {
	if (_alert) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_NULL(env);
		jstring jStrMessage = env->NewStringUTF(p_message.utf8().get_data());
		jstring jStrTitle = env->NewStringUTF(p_title.utf8().get_data());
		env->CallVoidMethod(noeengine_instance, _alert, jStrMessage, jStrTitle);
	}
}

int NoeEngineJavaWrapper::get_gles_version_code() {
	JNIEnv *env = get_jni_env();
	ERR_FAIL_NULL_V(env, 0);
	if (_get_GLES_version_code) {
		return env->CallIntMethod(noeengine_instance, _get_GLES_version_code);
	}
	return 0;
}

bool NoeEngineJavaWrapper::has_get_clipboard() {
	return _get_clipboard != nullptr;
}

String NoeEngineJavaWrapper::get_clipboard() {
	if (_get_clipboard) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_NULL_V(env, String());
		jstring s = (jstring)env->CallObjectMethod(noeengine_instance, _get_clipboard);
		return jstring_to_string(s, env);
	} else {
		return String();
	}
}

String NoeEngineJavaWrapper::get_input_fallback_mapping() {
	if (_get_input_fallback_mapping) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_NULL_V(env, String());
		jstring fallback_mapping = (jstring)env->CallObjectMethod(noeengine_instance, _get_input_fallback_mapping);
		return jstring_to_string(fallback_mapping, env);
	} else {
		return String();
	}
}

bool NoeEngineJavaWrapper::has_set_clipboard() {
	return _set_clipboard != nullptr;
}

void NoeEngineJavaWrapper::set_clipboard(const String &p_text) {
	if (_set_clipboard) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_NULL(env);
		jstring jStr = env->NewStringUTF(p_text.utf8().get_data());
		env->CallVoidMethod(noeengine_instance, _set_clipboard, jStr);
	}
}

bool NoeEngineJavaWrapper::has_has_clipboard() {
	return _has_clipboard != nullptr;
}

bool NoeEngineJavaWrapper::has_clipboard() {
	if (_has_clipboard) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_NULL_V(env, false);
		return env->CallBooleanMethod(noeengine_instance, _has_clipboard);
	} else {
		return false;
	}
}

bool NoeEngineJavaWrapper::request_permission(const String &p_name) {
	if (_request_permission) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_NULL_V(env, false);
		jstring jStrName = env->NewStringUTF(p_name.utf8().get_data());
		return env->CallBooleanMethod(noeengine_instance, _request_permission, jStrName);
	} else {
		return false;
	}
}

bool NoeEngineJavaWrapper::request_permissions() {
	if (_request_permissions) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_NULL_V(env, false);
		return env->CallBooleanMethod(noeengine_instance, _request_permissions);
	} else {
		return false;
	}
}

Vector<String> NoeEngineJavaWrapper::get_granted_permissions() const {
	Vector<String> permissions_list;
	if (_get_granted_permissions) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_NULL_V(env, permissions_list);
		jobject permissions_object = env->CallObjectMethod(noeengine_instance, _get_granted_permissions);
		jobjectArray *arr = reinterpret_cast<jobjectArray *>(&permissions_object);

		jsize len = env->GetArrayLength(*arr);
		for (int i = 0; i < len; i++) {
			jstring jstr = (jstring)env->GetObjectArrayElement(*arr, i);
			String str = jstring_to_string(jstr, env);
			permissions_list.push_back(str);
			env->DeleteLocalRef(jstr);
		}
	}
	return permissions_list;
}

void NoeEngineJavaWrapper::init_input_devices() {
	if (_init_input_devices) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_NULL(env);
		env->CallVoidMethod(noeengine_instance, _init_input_devices);
	}
}

jobject NoeEngineJavaWrapper::get_surface() {
	if (_get_surface) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_NULL_V(env, nullptr);
		return env->CallObjectMethod(noeengine_instance, _get_surface);
	} else {
		return nullptr;
	}
}

bool NoeEngineJavaWrapper::is_activity_resumed() {
	if (_is_activity_resumed) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_NULL_V(env, false);
		return env->CallBooleanMethod(noeengine_instance, _is_activity_resumed);
	} else {
		return false;
	}
}

void NoeEngineJavaWrapper::vibrate(int p_duration_ms) {
	if (_vibrate) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_NULL(env);
		env->CallVoidMethod(noeengine_instance, _vibrate, p_duration_ms);
	}
}

int NoeEngineJavaWrapper::create_new_noeengine_instance(List<String> args) {
	if (_create_new_noeengine_instance) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_NULL_V(env, 0);
		jobjectArray jargs = env->NewObjectArray(args.size(), env->FindClass("java/lang/String"), env->NewStringUTF(""));
		for (int i = 0; i < args.size(); i++) {
			env->SetObjectArrayElement(jargs, i, env->NewStringUTF(args[i].utf8().get_data()));
		}
		return env->CallIntMethod(noeengine_instance, _create_new_noeengine_instance, jargs);
	} else {
		return 0;
	}
}
