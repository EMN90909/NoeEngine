// Copyright (c) 2026 Noethric.
// SPDX-License-Identifier: LicenseRef-Noethric-Proprietary
#include "noqeri_script.h"
#include "noqeri_translator.h"

#include "core/io/file_access.h"
#include "core/object/class_db.h"

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

static int _noqeri_original_line(const Vector<int> &p_line_map, int p_compiled_line) {
    if (p_compiled_line > 0 && p_compiled_line <= p_line_map.size()) {
        return p_line_map[p_compiled_line - 1];
    }
    return p_compiled_line;
}

bool NoqeriLanguage::validate(const String &p_source, const String &p_path,
        List<String> *r_functions, List<ScriptLanguage::ScriptError> *r_errors,
        List<ScriptLanguage::Warning> *r_warnings, HashSet<int> *r_safe_lines) const {
    String converted;
    String message;
    Vector<int> line_map;
    int native_error_line = 1;
    if (!noqeri_translate_game_script(p_source, converted, message, &line_map, &native_error_line)) {
        if (r_errors) {
            ScriptError error;
            error.line = native_error_line;
            error.column = 1;
            error.message = message;
            r_errors->push_back(error);
        }
        return false;
    }

    // Godot 4.0's GDScript validator unconditionally appends to its function
    // list. Use local non-null collections, then map lines back to .nqr.
    List<String> translated_functions;
    List<ScriptError> translated_errors;
    List<Warning> translated_warnings;
    HashSet<int> translated_safe_lines;
    const bool ok = GDScriptLanguage::get_singleton()->validate(converted, p_path,
            &translated_functions, &translated_errors, &translated_warnings, &translated_safe_lines);

    if (r_functions) {
        for (const String &function : translated_functions) {
            const int colon = function.rfind(":");
            if (colon >= 0) {
                const int compiled_line = function.substr(colon + 1).to_int();
                r_functions->push_back(function.substr(0, colon + 1) +
                        itos(_noqeri_original_line(line_map, compiled_line)));
            } else {
                r_functions->push_back(function);
            }
        }
    }
    if (r_errors) {
        for (const ScriptError &native_error : translated_errors) {
            ScriptError err = native_error;
            err.line = _noqeri_original_line(line_map, err.line);
            r_errors->push_back(err);
        }
    }
    if (r_warnings) {
        for (const Warning &native_warning : translated_warnings) {
            Warning warning = native_warning;
            warning.start_line = _noqeri_original_line(line_map, warning.start_line);
            warning.end_line = _noqeri_original_line(line_map, warning.end_line);
            r_warnings->push_back(warning);
        }
    }
    if (r_safe_lines) {
        for (const int &line : translated_safe_lines) {
            r_safe_lines->insert(_noqeri_original_line(line_map, line));
        }
    }
    return ok;
}

Vector<ScriptLanguage::ScriptTemplate> NoqeriLanguage::get_built_in_templates(StringName p_object) {
    Vector<ScriptTemplate> templates;
    if (ClassDB::class_exists(p_object) &&
            (p_object == "Node" || ClassDB::is_parent_class(p_object, "Node"))) {
        ScriptTemplate entry;
        entry.name = "Noqeri Node";
        entry.description = "A native .nqr game script with a scene-ready callback.";
        entry.inherit = String(p_object);
        entry.content = "function _ready(): void {\n    \n}\n";
        templates.push_back(entry);
    }
    return templates;
}

Ref<Script> NoqeriLanguage::make_template(const String &p_template, const String &p_class_name, const String &p_base_class_name) const {
    Ref<NoqeriScript> noqeri_script;
    noqeri_script.instantiate();
    String base = p_base_class_name;
    if (!ClassDB::class_exists(base) ||
            (base != "Node" && !ClassDB::is_parent_class(base, "Node"))) {
        base = "Node";
    }
    String body = p_template.strip_edges();
    if (body.is_empty()) {
        body = "function _ready(): void {\n    \n}";
    }
    noqeri_script->set_source_code("// noeengine:extends " + base + "\n" + body + "\n");
    noqeri_script->reload();
    return noqeri_script;
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
        // The const Ref exposes a const Script*. Copy it before calling a mutating method.
        Ref<Script> reloadable_script = p_script;
        reloadable_script->reload(p_soft_reload);
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
    Ref<NoqeriScript> noqeri_script;
    noqeri_script.instantiate();
    noqeri_script->set_path(p_original_path.is_empty() ? p_path : p_original_path, true);
    noqeri_script->set_source_code(source->get_as_utf8_string());
    // Keep invalid code loadable so the native editor can correct it.
    noqeri_script->reload();
    if (r_error) {
        *r_error = OK;
    }
    return noqeri_script;
}

Error ResourceFormatSaverNoqeri::save(const Ref<Resource> &p_resource, const String &p_path, uint32_t p_flags) {
    Ref<NoqeriScript> noqeri_script = p_resource;
    ERR_FAIL_COND_V(noqeri_script.is_null(), ERR_INVALID_PARAMETER);
    Error err = OK;
    Ref<FileAccess> file = FileAccess::open(p_path, FileAccess::WRITE, &err);
    if (file.is_null()) {
        return err;
    }
    file->store_string(noqeri_script->get_source_code());
    return file->get_error() == OK ? OK : ERR_CANT_CREATE;
}

void ResourceFormatSaverNoqeri::get_recognized_extensions(const Ref<Resource> &p_resource, List<String> *p_extensions) const {
    if (recognize(p_resource)) {
        p_extensions->push_back("nqr");
    }
}
bool ResourceFormatSaverNoqeri::recognize(const Ref<Resource> &p_resource) const {
    Ref<NoqeriScript> noqeri_script = p_resource;
    return noqeri_script.is_valid();
}
