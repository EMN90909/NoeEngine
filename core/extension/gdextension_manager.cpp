// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "gdextension_manager.h"
#include "core/io/file_access.h"

GDExtensionManager::LoadStatus GDExtensionManager::load_extension(const String &p_path) {
	if (gdextension_map.has(p_path)) {
		return LOAD_STATUS_ALREADY_LOADED;
	}
	Ref<GDExtension> extension = ResourceLoader::load(p_path);
	if (extension.is_null()) {
		return LOAD_STATUS_FAILED;
	}

	if (level >= 0) { // Already initialized up to some level.
		int32_t minimum_level = extension->get_minimum_library_initialization_level();
		if (minimum_level < MIN(level, GDExtension::INITIALIZATION_LEVEL_SCENE)) {
			return LOAD_STATUS_NEEDS_RESTART;
		}
		// Initialize up to current level.
		for (int32_t i = minimum_level; i <= level; i++) {
			extension->initialize_library(GDExtension::InitializationLevel(i));
		}
	}
	gdextension_map[p_path] = extension;
	return LOAD_STATUS_OK;
}

GDExtensionManager::LoadStatus GDExtensionManager::reload_extension(const String &p_path) {
	return LOAD_STATUS_OK; //TODO
}
GDExtensionManager::LoadStatus GDExtensionManager::unload_extension(const String &p_path) {
	if (!gdextension_map.has(p_path)) {
		return LOAD_STATUS_NOT_LOADED;
	}

	Ref<GDExtension> extension = gdextension_map[p_path];

	if (level >= 0) { // Already initialized up to some level.
		int32_t minimum_level = extension->get_minimum_library_initialization_level();
		if (minimum_level < MIN(level, GDExtension::INITIALIZATION_LEVEL_SCENE)) {
			return LOAD_STATUS_NEEDS_RESTART;
		}
		// Deinitialize down to current level.
		for (int32_t i = level; i >= minimum_level; i--) {
			extension->deinitialize_library(GDExtension::InitializationLevel(i));
		}
	}
	gdextension_map.erase(p_path);
	return LOAD_STATUS_OK;
}

bool GDExtensionManager::is_extension_loaded(const String &p_path) const {
	return gdextension_map.has(p_path);
}

Vector<String> GDExtensionManager::get_loaded_extensions() const {
	Vector<String> ret;
	for (const KeyValue<String, Ref<GDExtension>> &E : gdextension_map) {
		ret.push_back(E.key);
	}
	return ret;
}
Ref<GDExtension> GDExtensionManager::get_extension(const String &p_path) {
	HashMap<String, Ref<GDExtension>>::Iterator E = gdextension_map.find(p_path);
	ERR_FAIL_COND_V(!E, Ref<GDExtension>());
	return E->value;
}

void GDExtensionManager::initialize_extensions(GDExtension::InitializationLevel p_level) {
	ERR_FAIL_COND(int32_t(p_level) - 1 != level);
	for (KeyValue<String, Ref<GDExtension>> &E : gdextension_map) {
		E.value->initialize_library(p_level);
	}
	level = p_level;
}

void GDExtensionManager::deinitialize_extensions(GDExtension::InitializationLevel p_level) {
	ERR_FAIL_COND(int32_t(p_level) != level);
	for (KeyValue<String, Ref<GDExtension>> &E : gdextension_map) {
		E.value->deinitialize_library(p_level);
	}
	level = int32_t(p_level) - 1;
}

void GDExtensionManager::load_extensions() {
	Ref<FileAccess> f = FileAccess::open(GDExtension::get_extension_list_config_file(), FileAccess::READ);
	while (f.is_valid() && !f->eof_reached()) {
		String s = f->get_line().strip_edges();
		if (!s.is_empty()) {
			LoadStatus err = load_extension(s);
			ERR_CONTINUE_MSG(err == LOAD_STATUS_FAILED, "Error loading extension: " + s);
		}
	}
}

GDExtensionManager *GDExtensionManager::get_singleton() {
	return singleton;
}
void GDExtensionManager::_bind_methods() {
	ClassDB::bind_method(D_METHOD("load_extension", "path"), &GDExtensionManager::load_extension);
	ClassDB::bind_method(D_METHOD("reload_extension", "path"), &GDExtensionManager::reload_extension);
	ClassDB::bind_method(D_METHOD("unload_extension", "path"), &GDExtensionManager::unload_extension);
	ClassDB::bind_method(D_METHOD("is_extension_loaded", "path"), &GDExtensionManager::is_extension_loaded);

	ClassDB::bind_method(D_METHOD("get_loaded_extensions"), &GDExtensionManager::get_loaded_extensions);
	ClassDB::bind_method(D_METHOD("get_extension", "path"), &GDExtensionManager::get_extension);

	BIND_ENUM_CONSTANT(LOAD_STATUS_OK);
	BIND_ENUM_CONSTANT(LOAD_STATUS_FAILED);
	BIND_ENUM_CONSTANT(LOAD_STATUS_ALREADY_LOADED);
	BIND_ENUM_CONSTANT(LOAD_STATUS_NOT_LOADED);
	BIND_ENUM_CONSTANT(LOAD_STATUS_NEEDS_RESTART);
}

GDExtensionManager *GDExtensionManager::singleton = nullptr;

GDExtensionManager::GDExtensionManager() {
	ERR_FAIL_COND(singleton != nullptr);
	singleton = this;
}
