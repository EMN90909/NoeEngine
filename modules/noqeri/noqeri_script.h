// Copyright (c) 2026 Noethric.
// SPDX-License-Identifier: LicenseRef-Noethric-Proprietary
#ifndef NOEENGINE_NOQERI_SCRIPT_H
#define NOEENGINE_NOQERI_SCRIPT_H

#include "modules/gdscript/gdscript.h"
#include "core/io/resource_loader.h"
#include "core/io/resource_saver.h"

// Native .nqr Script resource. The first scene bridge executes a documented,
// restricted subset through the engine's existing GDScript VM. It is NOT
// Noqeri's complete GPL-3.0-only compiler or standalone runtime.
class NoqeriScript : public GDScript {
    GDCLASS(NoqeriScript, GDScript);

    String noqeri_source;

protected:
    static void _bind_methods() {}

public:
    String get_source_code() const override { return noqeri_source; }
    void set_source_code(const String &p_code) override { noqeri_source = p_code; }
    bool has_source_code() const override { return !noqeri_source.is_empty(); }
    Error reload(bool p_keep_state = false) override;
    ScriptLanguage *get_language() const override;
};

class NoqeriLanguage : public ScriptLanguage {
    GDCLASS(NoqeriLanguage, ScriptLanguage);

protected:
    static void _bind_methods() {}

public:
    String get_name() const override { return "Noqeri"; }
    String get_type() const override { return "NoqeriScript"; }
    String get_extension() const override { return "nqr"; }
    void init() override {}
    void finish() override {}

    void get_reserved_words(List<String> *p_words) const override;
    bool is_control_flow_keyword(String p_word) const override;
    void get_comment_delimiters(List<String> *p_delimiters) const override;
    void get_string_delimiters(List<String> *p_delimiters) const override;
    bool validate(const String &p_script, const String &p_path = "", List<String> *r_functions = nullptr,
            List<ScriptLanguage::ScriptError> *r_errors = nullptr,
            List<ScriptLanguage::Warning> *r_warnings = nullptr,
            HashSet<int> *r_safe_lines = nullptr) const override;
    Script *create_script() const override;
    bool is_using_templates() override { return true; }
    Vector<ScriptTemplate> get_built_in_templates(StringName p_object) override;
    Ref<Script> make_template(const String &p_template, const String &p_class_name, const String &p_base_class_name) const override;
    bool has_named_classes() const override { return false; }
    bool supports_builtin_mode() const override { return false; }
    int find_function(const String &p_function, const String &p_code) const override;
    String make_function(const String &p_class, const String &p_name, const PackedStringArray &p_args) const override;
    void auto_indent_code(String &p_code, int p_from_line, int p_to_line) const override {}
    void add_global_constant(const StringName &p_name, const Variant &p_value) override;
    void add_named_global_constant(const StringName &p_name, const Variant &p_value) override;
    void remove_named_global_constant(const StringName &p_name) override;

    String debug_get_error() const override;
    int debug_get_stack_level_count() const override;
    int debug_get_stack_level_line(int p_level) const override;
    String debug_get_stack_level_function(int p_level) const override;
    String debug_get_stack_level_source(int p_level) const override;
    void debug_get_stack_level_locals(int p_level, List<String> *p_locals, List<Variant> *p_values, int p_max_subitems = -1, int p_max_depth = -1) override;
    void debug_get_stack_level_members(int p_level, List<String> *p_members, List<Variant> *p_values, int p_max_subitems = -1, int p_max_depth = -1) override;
    void debug_get_globals(List<String> *p_globals, List<Variant> *p_values, int p_max_subitems = -1, int p_max_depth = -1) override;
    String debug_parse_stack_level_expression(int p_level, const String &p_expression, int p_max_subitems = -1, int p_max_depth = -1) override;

    void reload_all_scripts() override;
    void reload_tool_script(const Ref<Script> &p_script, bool p_soft_reload) override;

    void get_recognized_extensions(List<String> *p_extensions) const override { p_extensions->push_back("nqr"); }
    void get_public_functions(List<MethodInfo> *p_functions) const override;
    void get_public_constants(List<Pair<String, Variant>> *p_constants) const override;
    void get_public_annotations(List<MethodInfo> *p_annotations) const override;

    void profiling_start() override;
    void profiling_stop() override;
    int profiling_get_accumulated_data(ProfilingInfo *p_arr, int p_max) override;
    int profiling_get_frame_data(ProfilingInfo *p_arr, int p_max) override;
};

class ResourceFormatLoaderNoqeri : public ResourceFormatLoader {
public:
    Ref<Resource> load(const String &p_path, const String &p_original_path = "",
            Error *r_error = nullptr, bool p_use_sub_threads = false,
            float *r_progress = nullptr, CacheMode p_cache_mode = CACHE_MODE_REUSE) override;
    void get_recognized_extensions(List<String> *p_extensions) const override { p_extensions->push_back("nqr"); }
    bool handles_type(const String &p_type) const override { return p_type == "Script" || p_type == "NoqeriScript"; }
    String get_resource_type(const String &p_path) const override { return p_path.get_extension().to_lower() == "nqr" ? "NoqeriScript" : ""; }
};

class ResourceFormatSaverNoqeri : public ResourceFormatSaver {
public:
    Error save(const Ref<Resource> &p_resource, const String &p_path, uint32_t p_flags = 0) override;
    void get_recognized_extensions(const Ref<Resource> &p_resource, List<String> *p_extensions) const override;
    bool recognize(const Ref<Resource> &p_resource) const override;
};

extern NoqeriLanguage *script_language_noqeri;

#endif
