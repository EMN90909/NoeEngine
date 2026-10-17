// Copyright (c) 2026 Noethric.
// SPDX-License-Identifier: LicenseRef-Noethric-Proprietary
#include "noqeri_script.h"
#include "noqeri_translator.h"

#include "core/io/file_access.h"

NoqeriLanguage *script_language_noqeri = nullptr;

Error NoqeriScript::reload(bool p_keep_state) {
    String translated;
    String message;
    if (!noqeri_translate_game_script(noqeri_source, translated, message)) {
        ERR_PRINT("Noqeri scene script: " + message);
        // Make sure a formerly valid script never keeps executing stale code.
        GDScript::set_source_code("extends Node\n__NOQERI_INVALID_SCENE_SCRIPT__\n");
        GDScript::reload(p_keep_state);
        return ERR_PARSE_ERROR;
    }
    // Godot's existing VM provides instance lifecycle, Node calls and
    // notifications. Full Noqeri semantics are intentionally NOT claimed.
    GDScript::set_source_code(translated);
    return GDScript::reload(p_keep_state);
}

ScriptLanguage *NoqeriScript::get_language() const {
    return script_language_noqeri;
}

void NoqeriLanguage::get_reserved_words(List<String> *p_words) const {
    const char *words[] = {
        "function", "let", "const", "return", "if", "else", "while",
        "true", "false", "null", "module", "import", "record", "export",
        "extern", "unsafe", "try", "throw", "repeat", "void",
    };
    for (const char *word : words) {
        p_words->push_back(word);
    }
}

bool NoqeriLanguage::is_control_flow_keyword(String p_word) const {
    return p_word == "if" || p_word == "else" || p_word == "while" ||
            p_word == "return" || p_word == "repeat" || p_word == "throw";
}

void NoqeriLanguage::get_comment_delimiters(List<String> *p_delimiters) const {
    p_delimiters->push_back("//");
}

void NoqeriLanguage::get_string_delimiters(List<String> *p_delimiters) const {
    p_delimiters->push_back("\" \"");
}

bool NoqeriLanguage::validate(const String &p_source, const String &p_path,
        List<String> *r_functions, List<ScriptLanguage::ScriptError> *r_errors,
        List<ScriptLanguage::Warning> *r_warnings, HashSet<int> *r_safe_lines) const {
    String converted;
    String message;
    if (!noqeri_translate_game_script(p_source, converted, message)) {
        if (r_errors) {
            ScriptError error;
            error.line = 1;
            error.column = 1;
            error.message = message;
            r_errors->push_back(error);
        }
        return false;
    }
    return GDScriptLanguage::get_singleton()->validate(converted, p_path,
            r_functions, r_errors, r_warnings, r_safe_lines);
}

Script *NoqeriLanguage::create_script() const {
    return memnew(NoqeriScript);
}

int NoqeriLanguage::find_function(const String &p_function, const String &p_code) const {
    const int offset = p_code.find("function " + p_function + "(");
    if (offset < 0) {
        return -1;
    }
    return p_code.substr(0, offset).count("\n");
}

String NoqeriLanguage::make_function(const String &p_class, const String &p_name, const PackedStringArray &p_args) const {
    String args;
    for (int i = 0; i < p_args.size(); ++i) {
        if (i > 0) { args += ", "; }
        args += p_args[i];
    }
    return "function " + p_name + "(" + args + "): void {\n    \n}\n";
}

void NoqeriLanguage::add_global_constant(const StringName &p_name, const Variant &p_value) {
    GDScriptLanguage::get_singleton()->add_global_constant(p_name, p_value);
}

void NoqeriLanguage::add_named_global_constant(const StringName &p_name, const Variant &p_value) {
    GDScriptLanguage::get_singleton()->add_named_global_constant(p_name, p_value);
}

void NoqeriLanguage::remove_named_global_constant(const StringName &p_name) {
    GDScriptLanguage::get_singleton()->remove_named_global_constant(p_name);
}

String NoqeriLanguage::debug_get_error() const {
    return GDScriptLanguage::get_singleton()->debug_get_error();
}
int NoqeriLanguage::debug_get_stack_level_count() const {
    return GDScriptLanguage::get_singleton()->debug_get_stack_level_count();
}
int NoqeriLanguage::debug_get_stack_level_line(int p_level) const {
    return GDScriptLanguage::get_singleton()->debug_get_stack_level_line(p_level);
}
String NoqeriLanguage::debug_get_stack_level_function(int p_level) const {
    return GDScriptLanguage::get_singleton()->debug_get_stack_level_function(p_level);
}
String NoqeriLanguage::debug_get_stack_level_source(int p_level) const {
    return GDScriptLanguage::get_singleton()->debug_get_stack_level_source(p_level);
}
void NoqeriLanguage::debug_get_stack_level_locals(int p_level, List<String> *p_locals, List<Variant> *p_values, int p_max_subitems, int p_max_depth) {
    GDScriptLanguage::get_singleton()->debug_get_stack_level_locals(p_level, p_locals, p_values, p_max_subitems, p_max_depth);
}
void NoqeriLanguage::debug_get_stack_level_members(int p_level, List<String> *p_members, List<Variant> *p_values, int p_max_subitems, int p_max_depth) {
    GDScriptLanguage::get_singleton()->debug_get_stack_level_members(p_level, p_members, p_values, p_max_subitems, p_max_depth);
}
void NoqeriLanguage::debug_get_globals(List<String> *p_globals, List<Variant> *p_values, int p_max_subitems, int p_max_depth) {
    GDScriptLanguage::get_singleton()->debug_get_globals(p_globals, p_values, p_max_subitems, p_max_depth);
}
String NoqeriLanguage::debug_parse_stack_level_expression(int p_level, const String &p_expression, int p_max_subitems, int p_max_depth) {
    return GDScriptLanguage::get_singleton()->debug_parse_stack_level_expression(p_level, p_expression, p_max_subitems, p_max_depth);
}

void NoqeriLanguage::reload_all_scripts() {
    GDScriptLanguage::get_singleton()->reload_all_scripts();
}
void NoqeriLanguage::reload_tool_script(const Ref<Script> &p_script, bool p_soft_reload) {
    if (p_script.is_valid()) {
        p_script->reload(p_soft_reload);
    }
}
void NoqeriLanguage::get_public_functions(List<MethodInfo> *p_functions) const {
    GDScriptLanguage::get_singleton()->get_public_functions(p_functions);
}
void NoqeriLanguage::get_public_constants(List<Pair<String, Variant>> *p_constants) const {
    GDScriptLanguage::get_singleton()->get_public_constants(p_constants);
}
void NoqeriLanguage::get_public_annotations(List<MethodInfo> *p_annotations) const {}
void NoqeriLanguage::profiling_start() {
    GDScriptLanguage::get_singleton()->profiling_start();
}
void NoqeriLanguage::profiling_stop() {
    GDScriptLanguage::get_singleton()->profiling_stop();
}
int NoqeriLanguage::profiling_get_accumulated_data(ProfilingInfo *p_arr, int p_max) {
    return GDScriptLanguage::get_singleton()->profiling_get_accumulated_data(p_arr, p_max);
}
int NoqeriLanguage::profiling_get_frame_data(ProfilingInfo *p_arr, int p_max) {
    return GDScriptLanguage::get_singleton()->profiling_get_frame_data(p_arr, p_max);
}

Ref<Resource> ResourceFormatLoaderNoqeri::load(const String &p_path, const String &p_original_path,
        Error *r_error, bool p_use_sub_threads, float *r_progress, CacheMode p_cache_mode) {
    Error err = OK;
    Ref<FileAccess> source = FileAccess::open(p_path, FileAccess::READ, &err);
    if (source.is_null()) {
        if (r_error) {
            *r_error = err;
        }
        return Ref<Resource>();
    }
    Ref<NoqeriScript> script;
    script.instantiate();
    script->set_path(p_original_path.is_empty() ? p_path : p_original_path, true);
    script->set_source_code(source->get_as_utf8_string());
    // Keep invalid code loadable so the native editor can correct it.
    script->reload();
    if (r_error) {
        *r_error = OK;
    }
    return script;
}

Error ResourceFormatSaverNoqeri::save(const Ref<Resource> &p_resource, const String &p_path, uint32_t p_flags) {
    Ref<NoqeriScript> script = p_resource;
    ERR_FAIL_COND_V(script.is_null(), ERR_INVALID_PARAMETER);
    Error err = OK;
    Ref<FileAccess> file = FileAccess::open(p_path, FileAccess::WRITE, &err);
    if (file.is_null()) {
        return err;
    }
    file->store_string(script->get_source_code());
    return file->get_error() == OK ? OK : ERR_CANT_CREATE;
}

void ResourceFormatSaverNoqeri::get_recognized_extensions(const Ref<Resource> &p_resource, List<String> *p_extensions) const {
    if (recognize(p_resource)) {
        p_extensions->push_back("nqr");
    }
}
bool ResourceFormatSaverNoqeri::recognize(const Ref<Resource> &p_resource) const {
    Ref<NoqeriScript> script = p_resource;
    return script.is_valid();
}
