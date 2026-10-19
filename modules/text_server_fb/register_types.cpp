// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "register_types.h"

#include "text_server_fb.h"

void initialize_text_server_fb_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SERVERS) {
		return;
	}

	GDREGISTER_CLASS(TextServerFallback);
	TextServerManager *tsman = TextServerManager::get_singleton();
	if (tsman) {
		Ref<TextServerFallback> ts;
		ts.instantiate();
		tsman->add_interface(ts);
	}
}

void uninitialize_text_server_fb_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SERVERS) {
		return;
	}
}

#ifdef GDEXTENSION

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/core/memory.hpp>

using namespace godot;

extern "C" {

GDExtensionBool GDE_EXPORT textserver_fallback_init(const GDExtensionInterface *p_interface, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	GDExtensionBinding::InitObject init_obj(p_interface, p_library, r_initialization);

	init_obj.register_initializer(&initialize_text_server_fb_module);
	init_obj.register_terminator(&uninitialize_text_server_fb_module);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SERVERS);

	return init_obj.init();
}

} // ! extern "C"

#endif // ! GDEXTENSION
