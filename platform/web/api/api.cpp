// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "api.h"
#include "core/config/engine.h"
#include "javascript_bridge_singleton.h"
#include "web_tools_editor_plugin.h"

static JavaScriptBridge *javascript_bridge_singleton;

void register_web_api() {
	WebToolsEditorPlugin::initialize();
	GDREGISTER_ABSTRACT_CLASS(JavaScriptObject);
	GDREGISTER_ABSTRACT_CLASS(JavaScriptBridge);
	javascript_bridge_singleton = memnew(JavaScriptBridge);
	Engine::get_singleton()->add_singleton(Engine::Singleton("JavaScriptBridge", javascript_bridge_singleton));
}

void unregister_web_api() {
	memdelete(javascript_bridge_singleton);
}

JavaScriptBridge *JavaScriptBridge::singleton = nullptr;

JavaScriptBridge *JavaScriptBridge::get_singleton() {
	return singleton;
}

JavaScriptBridge::JavaScriptBridge() {
	ERR_FAIL_COND_MSG(singleton != nullptr, "JavaScriptBridge singleton already exist.");
	singleton = this;
}

JavaScriptBridge::~JavaScriptBridge() {}

void JavaScriptBridge::_bind_methods() {
	ClassDB::bind_method(D_METHOD("eval", "code", "use_global_execution_context"), &JavaScriptBridge::eval, DEFVAL(false));
	ClassDB::bind_method(D_METHOD("get_interface", "interface"), &JavaScriptBridge::get_interface);
	ClassDB::bind_method(D_METHOD("create_callback", "callable"), &JavaScriptBridge::create_callback);
	{
		MethodInfo mi;
		mi.name = "create_object";
		mi.arguments.push_back(PropertyInfo(Variant::STRING, "object"));
		ClassDB::bind_vararg_method(METHOD_FLAGS_DEFAULT, "create_object", &JavaScriptBridge::_create_object_bind, mi);
	}
	ClassDB::bind_method(D_METHOD("download_buffer", "buffer", "name", "mime"), &JavaScriptBridge::download_buffer, DEFVAL("application/octet-stream"));
	ClassDB::bind_method(D_METHOD("pwa_needs_update"), &JavaScriptBridge::pwa_needs_update);
	ClassDB::bind_method(D_METHOD("pwa_update"), &JavaScriptBridge::pwa_update);
	ClassDB::bind_method(D_METHOD("force_fs_sync"), &JavaScriptBridge::force_fs_sync);
	ADD_SIGNAL(MethodInfo("pwa_update_available"));
}

#if !defined(WEB_ENABLED) || !defined(JAVASCRIPT_EVAL_ENABLED)
Variant JavaScriptBridge::eval(const String &p_code, bool p_use_global_exec_context) {
	return Variant();
}

Ref<JavaScriptObject> JavaScriptBridge::get_interface(const String &p_interface) {
	return Ref<JavaScriptObject>();
}

Ref<JavaScriptObject> JavaScriptBridge::create_callback(const Callable &p_callable) {
	return Ref<JavaScriptObject>();
}

Variant JavaScriptBridge::_create_object_bind(const Variant **p_args, int p_argcount, Callable::CallError &r_error) {
	if (p_argcount < 1) {
		r_error.error = Callable::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
		r_error.argument = 0;
		return Ref<JavaScriptObject>();
	}
	if (p_args[0]->get_type() != Variant::STRING) {
		r_error.error = Callable::CallError::CALL_ERROR_INVALID_ARGUMENT;
		r_error.argument = 0;
		r_error.expected = Variant::STRING;
		return Ref<JavaScriptObject>();
	}
	return Ref<JavaScriptObject>();
}
#endif
#if !defined(WEB_ENABLED)
bool JavaScriptBridge::pwa_needs_update() const {
	return false;
}
Error JavaScriptBridge::pwa_update() {
	return ERR_UNAVAILABLE;
}
void JavaScriptBridge::force_fs_sync() {
}
void JavaScriptBridge::download_buffer(Vector<uint8_t> p_arr, const String &p_name, const String &p_mime) {
}
#endif
