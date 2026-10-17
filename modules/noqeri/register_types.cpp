// Copyright (c) 2026 Noethric.
// SPDX-License-Identifier: LicenseRef-Noethric-Proprietary
#include "register_types.h"
#include "noqeri_script.h"

#include "core/io/resource_loader.h"
#include "core/io/resource_saver.h"

static Ref<ResourceFormatLoaderNoqeri> noqeri_loader;
static Ref<ResourceFormatSaverNoqeri> noqeri_saver;

void initialize_noqeri_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SERVERS) {
        return;
    }
    GDREGISTER_CLASS(NoqeriScript);
    script_language_noqeri = memnew(NoqeriLanguage);
    const Error language_status = ScriptServer::register_language(script_language_noqeri);
    if (language_status != OK) {
        memdelete(script_language_noqeri);
        script_language_noqeri = nullptr;
        return;
    }
    noqeri_loader.instantiate();
    noqeri_saver.instantiate();
    ResourceLoader::add_resource_format_loader(noqeri_loader);
    ResourceSaver::add_resource_format_saver(noqeri_saver);
}

void uninitialize_noqeri_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SERVERS) {
        return;
    }
    if (noqeri_loader.is_valid()) {
        ResourceLoader::remove_resource_format_loader(noqeri_loader);
        noqeri_loader.unref();
    }
    if (noqeri_saver.is_valid()) {
        ResourceSaver::remove_resource_format_saver(noqeri_saver);
        noqeri_saver.unref();
    }
    if (script_language_noqeri != nullptr) {
        ScriptServer::unregister_language(script_language_noqeri);
        memdelete(script_language_noqeri);
        script_language_noqeri = nullptr;
    }
}
