// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef GDEXTENSION_H
#define GDEXTENSION_H

#include <functional>

#include "core/extension/gdextension_interface.h"
#include "core/io/config_file.h"
#include "core/io/resource_loader.h"
#include "core/object/ref_counted.h"

class GDExtension : public Resource {
	GDCLASS(GDExtension, Resource)

	void *library = nullptr; // pointer if valid,
	String library_path;

	struct Extension {
		ObjectGDExtension gdextension;
	};

	HashMap<StringName, Extension> extension_classes;

	static void _register_extension_class(GDExtensionClassLibraryPtr p_library, GDExtensionConstStringNamePtr p_class_name, GDExtensionConstStringNamePtr p_parent_class_name, const GDExtensionClassCreationInfo *p_extension_funcs);
	static void _register_extension_class_method(GDExtensionClassLibraryPtr p_library, GDExtensionConstStringNamePtr p_class_name, const GDExtensionClassMethodInfo *p_method_info);
	static void _register_extension_class_integer_constant(GDExtensionClassLibraryPtr p_library, GDExtensionConstStringNamePtr p_class_name, GDExtensionConstStringNamePtr p_enum_name, GDExtensionConstStringNamePtr p_constant_name, GDExtensionInt p_constant_value, GDExtensionBool p_is_bitfield);
	static void _register_extension_class_property(GDExtensionClassLibraryPtr p_library, GDExtensionConstStringNamePtr p_class_name, const GDExtensionPropertyInfo *p_info, GDExtensionConstStringNamePtr p_setter, GDExtensionConstStringNamePtr p_getter);
	static void _register_extension_class_property_group(GDExtensionClassLibraryPtr p_library, GDExtensionConstStringNamePtr p_class_name, GDExtensionConstStringNamePtr p_group_name, GDExtensionConstStringNamePtr p_prefix);
	static void _register_extension_class_property_subgroup(GDExtensionClassLibraryPtr p_library, GDExtensionConstStringNamePtr p_class_name, GDExtensionConstStringNamePtr p_subgroup_name, GDExtensionConstStringNamePtr p_prefix);
	static void _register_extension_class_signal(GDExtensionClassLibraryPtr p_library, GDExtensionConstStringNamePtr p_class_name, GDExtensionConstStringNamePtr p_signal_name, const GDExtensionPropertyInfo *p_argument_info, GDExtensionInt p_argument_count);
	static void _unregister_extension_class(GDExtensionClassLibraryPtr p_library, GDExtensionConstStringNamePtr p_class_name);
	static void _get_library_path(GDExtensionClassLibraryPtr p_library, GDExtensionStringPtr r_path);

	GDExtensionInitialization initialization;
	int32_t level_initialized = -1;

protected:
	static void _bind_methods();

public:
	static String get_extension_list_config_file();
	static String find_extension_library(const String &p_path, Ref<ConfigFile> p_config, std::function<bool(String)> p_has_feature, PackedStringArray *r_tags = nullptr);

	Error open_library(const String &p_path, const String &p_entry_symbol);
	void close_library();

	enum InitializationLevel {
		INITIALIZATION_LEVEL_CORE = GDEXTENSION_INITIALIZATION_CORE,
		INITIALIZATION_LEVEL_SERVERS = GDEXTENSION_INITIALIZATION_SERVERS,
		INITIALIZATION_LEVEL_SCENE = GDEXTENSION_INITIALIZATION_SCENE,
		INITIALIZATION_LEVEL_EDITOR = GDEXTENSION_INITIALIZATION_EDITOR
	};

	bool is_library_open() const;

	InitializationLevel get_minimum_library_initialization_level() const;
	void initialize_library(InitializationLevel p_level);
	void deinitialize_library(InitializationLevel p_level);

	static void initialize_gdextensions();
	GDExtension();
	~GDExtension();
};

VARIANT_ENUM_CAST(GDExtension::InitializationLevel)

class GDExtensionResourceLoader : public ResourceFormatLoader {
public:
	virtual Ref<Resource> load(const String &p_path, const String &p_original_path, Error *r_error, bool p_use_sub_threads = false, float *r_progress = nullptr, CacheMode p_cache_mode = CACHE_MODE_REUSE);
	virtual void get_recognized_extensions(List<String> *p_extensions) const;
	virtual bool handles_type(const String &p_type) const;
	virtual String get_resource_type(const String &p_path) const;
};

#endif // GDEXTENSION_H
