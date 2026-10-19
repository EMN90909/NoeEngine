// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "register_types.h"

#include "core/config/engine.h"

#include "csharp_script.h"

CSharpLanguage *script_language_cs = nullptr;
Ref<ResourceFormatLoaderCSharpScript> resource_loader_cs;
Ref<ResourceFormatSaverCSharpScript> resource_saver_cs;

mono_bind::GodotSharp *_godotsharp = nullptr;

void initialize_mono_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	GDREGISTER_CLASS(CSharpScript);

	_godotsharp = memnew(mono_bind::GodotSharp);

	GDREGISTER_CLASS(mono_bind::GodotSharp);
	Engine::get_singleton()->add_singleton(Engine::Singleton("GodotSharp", mono_bind::GodotSharp::get_singleton()));

	script_language_cs = memnew(CSharpLanguage);
	script_language_cs->set_language_index(ScriptServer::get_language_count());
	ScriptServer::register_language(script_language_cs);

	resource_loader_cs.instantiate();
	ResourceLoader::add_resource_format_loader(resource_loader_cs);

	resource_saver_cs.instantiate();
	ResourceSaver::add_resource_format_saver(resource_saver_cs);
}

void uninitialize_mono_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	ScriptServer::unregister_language(script_language_cs);

	if (script_language_cs) {
		memdelete(script_language_cs);
	}

	ResourceLoader::remove_resource_format_loader(resource_loader_cs);
	resource_loader_cs.unref();

	ResourceSaver::remove_resource_format_saver(resource_saver_cs);
	resource_saver_cs.unref();

	if (_godotsharp) {
		memdelete(_godotsharp);
	}
}
