// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef GDEXTENSION_MANAGER_H
#define GDEXTENSION_MANAGER_H

#include "core/extension/gdextension.h"

class GDExtensionManager : public Object {
	GDCLASS(GDExtensionManager, Object);

	int32_t level = -1;
	HashMap<String, Ref<GDExtension>> gdextension_map;

	static void _bind_methods();

	static GDExtensionManager *singleton;

public:
	enum LoadStatus {
		LOAD_STATUS_OK,
		LOAD_STATUS_FAILED,
		LOAD_STATUS_ALREADY_LOADED,
		LOAD_STATUS_NOT_LOADED,
		LOAD_STATUS_NEEDS_RESTART,
	};

	LoadStatus load_extension(const String &p_path);
	LoadStatus reload_extension(const String &p_path);
	LoadStatus unload_extension(const String &p_path);
	bool is_extension_loaded(const String &p_path) const;
	Vector<String> get_loaded_extensions() const;
	Ref<GDExtension> get_extension(const String &p_path);

	void initialize_extensions(GDExtension::InitializationLevel p_level);
	void deinitialize_extensions(GDExtension::InitializationLevel p_level);

	static GDExtensionManager *get_singleton();

	void load_extensions();

	GDExtensionManager();
};

VARIANT_ENUM_CAST(GDExtensionManager::LoadStatus)

#endif // GDEXTENSION_MANAGER_H
