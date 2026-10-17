// Copyright (c) 2026 Noethric.
// SPDX-License-Identifier: LicenseRef-Noethric-Proprietary
//
// Restricted .nqr scene-scripting bridge. This does not link the GPL-licensed
// Noqeri compiler. Unsupported language features deliberately fail closed.
#include "noqeri_translator.h"

#include "core/string/ustring.h"
#include "core/object/class_db.h"

namespace {

bool is_identifier_boundary(const String &p_line, const String &p_token) {
    return p_line == p_token || p_line.begins_with(p_token + " ");
}

String convert_basic_types(String p_line) {
    // Only types with corresponding Godot Variant semantics are accepted.
    p_line = p_line.replace(": string", ": String");
    p_line = p_line.replace("-> string", "-> String");
    return p_line;
}

bool transform_statement(String &r_line, bool p_block_header, String &r_error, int &r_repeat_counter) {
    String line = r_line.strip_edges();
    if (line.is_empty()) {
        r_line = String();
        return true;
    }

    // Genuine Noqeri systems constructs must NOT silently execute with
    // incompatible GDScript semantics.
    const char *unsupported[] = {
        "module", "import", "record", "extern", "export", "unsafe",
        "throw", "try", "volatile", "atomic", "struct",
        "union", "enum", "defer", "type", "trait", "static", "await",
        "match", "class_name", "extends", "breakpoint",
    };
    for (const char *term : unsupported) {
        if (is_identifier_boundary(line, String(term))) {
            r_error = "This Noqeri construct requires the native Noqeri runtime: " + String(term);
            return false;
        }
    }
    if (line.contains(" as ")) {
        r_error = "Noqeri cast semantics are not yet supported by the native scene bridge.";
        return false;
    }
    const char *widths[] = { ": u8", ": u16", ": u32", ": u64", ": i8", ": i16", ": i32", ": i64", ": usize", ": isize" };
    for (const char *width : widths) {
        if (line.contains(String(width))) {
            r_error = "Fixed-width Noqeri types are not supported by the initial scene VM bridge.";
            return false;
        }
    }

    if (line.begins_with("function ")) {
        if (!p_block_header) {
            r_error = "Noqeri functions must have a { ... } body in a scene script.";
            return false;
        }
        line = "func " + line.substr(9);
        const int close = line.find(")");
        if (close < 0) {
            r_error = "Expected ')' in function declaration.";
            return false;
        }
        const int colon = line.find(":", close + 1);
        if (colon >= 0) {
            const String result = line.substr(colon + 1).strip_edges();
            if (result.is_empty()) {
                r_error = "Expected a function return type after ':'.";
                return false;
            }
            line = line.substr(0, colon) + " -> " + result;
        }
    } else if (line.begins_with("let ")) {
        line = "var " + line.substr(4);
    } else if (line.begins_with("else if ")) {
        line = "elif " + line.substr(8);
    } else if (line.begins_with("repeat ")) {
        if (!p_block_header) {
            r_error = "Noqeri repeat requires a { ... } block.";
            return false;
        }
        const String times = line.substr(7).strip_edges();
        if (times.is_empty()) {
            r_error = "Noqeri repeat requires a count.";
            return false;
        }
        // Evaluate the count once, with zero iterations for negative counts.
        line = "for __nqr_repeat_" + itos(r_repeat_counter++) + " in range(max(0, int(" + times + ")))";
    }

    line = convert_basic_types(line);
    if (p_block_header) {
        if (!(line.begins_with("func ") || line.begins_with("if ") || line.begins_with("elif ") ||
                    line == "else" || line.begins_with("while ") || line.begins_with("for "))) {
            r_error = "Unsupported Noqeri block; supported blocks: function, if, else, while.";
            return false;
        }
        line += ":";
    } else if (line.begins_with("func ") || line.begins_with("if ") ||
            line.begins_with("while ") || line == "else" || line.begins_with("elif ") || line.begins_with("repeat ")) {
        r_error = "Expected '{' after a Noqeri block header.";
        return false;
    }

    r_line = line;
    return true;
}

bool emit_statement(const String &p_segment, int p_depth, bool p_block_header, String &r_output, String &r_error, int &r_statements, int &r_repeat_counter) {
    String stmt = p_segment.strip_edges();
    if (stmt.is_empty()) {
        if (p_block_header) {
            r_error = "Empty Noqeri block header.";
            return false;
        }
        return true;
    }
    if (!transform_statement(stmt, p_block_header, r_error, r_repeat_counter)) {
        return false;
    }
    if (stmt.is_empty()) {
        return true;
    }
    if (p_depth == 0) {
        // Scene script classes can declare variables, constants and methods,
        // but do not execute free-standing Noqeri program statements.
        // Reject GDScript-only top-level annotations too (e.g. @tool).
        const bool declaration = p_block_header ? stmt.begins_with("func ") :
                (stmt.begins_with("var ") || stmt.begins_with("const ") || stmt.begins_with("#"));
        if (!declaration) {
            r_error = "Top-level Noqeri scene script code must be inside a function. "
                      "Only function, let, const and comment declarations are allowed outside functions.";
            return false;
        }
    }
    for (int i = 0; i < p_depth; ++i) {
        r_output += "\t";
    }
    r_output += stmt + "\n";
    if (!stmt.begins_with("#")) {
        ++r_statements;
    }
    return true;
}

} // namespace

bool noqeri_translate_game_script(const String &p_source, String &r_compiled, String &r_error) {
    r_compiled = String();
    r_error = String();
    String native_base = "Node";
    const Vector<String> input_lines = p_source.split("\n");
    for (int i = 0; i < input_lines.size(); ++i) {
        const String trimmed = input_lines[i].strip_edges();
        if (trimmed.begins_with("// noeengine:extends ")) {
            const String desired = trimmed.substr(String("// noeengine:extends ").length()).strip_edges();
            if (!ClassDB::class_exists(desired) ||
                    (desired != "Node" && !ClassDB::is_parent_class(desired, "Node"))) {
                r_error = "Unsupported native scene base type in // noeengine:extends directive: " + desired;
                return false;
            }
            native_base = desired;
        }
    }

    r_compiled = "extends " + native_base + "\n";
    String segment;
    int depth = 0;
    bool in_string = false;
    bool escaped = false;
    bool in_comment = false;
    int statements = 0;
    int repeat_counter = 0;
    int block_start_count[64] = {};
    for (int i = 0; i < p_source.length(); ++i) {
        const char32_t c = p_source[i];
        if (c == '\r') {
            continue;
        }
        if (in_comment) {
            if (c == '\n') {
                in_comment = false;
                if (!emit_statement(segment, depth, false, r_compiled, r_error, statements, repeat_counter)) {
                    return false;
                }
                segment = String();
            } else {
                segment += String::chr(c);
            }
            continue;
        }
        if (in_string) {
            segment += String::chr(c);
            if (escaped) {
                escaped = false;
            } else if (c == '\\') {
                escaped = true;
            } else if (c == '"') {
                in_string = false;
            }
            continue;
        }
        if (c == '"') {
            in_string = true;
            segment += String::chr(c);
            continue;
        }
        if (c == '/' && i + 1 < p_source.length() && p_source[i + 1] == '*') {
            r_error = "Block comments are not supported in NoeEngine's initial .nqr scene bridge. Use //.";
            return false;
        }
        if (c == '/' && i + 1 < p_source.length() && p_source[i + 1] == '/') {
            segment += "#";
            ++i;
            in_comment = true;
            continue;
        }
        if (c == '{') {
            if (depth >= 63) {
                r_error = "Noqeri scene script exceeds maximum supported block depth.";
                return false;
            }
            if (!emit_statement(segment, depth, true, r_compiled, r_error, statements, repeat_counter)) {
                return false;
            }
            segment = String();
            block_start_count[depth] = statements;
            ++depth;
        } else if (c == '}') {
            if (!emit_statement(segment, depth, false, r_compiled, r_error, statements, repeat_counter)) {
                return false;
            }
            segment = String();
            if (depth == 0) {
                r_error = "Unmatched closing '}' in Noqeri scene script.";
                return false;
            }
            --depth;
            if (block_start_count[depth] == statements) {
                for (int n = 0; n < depth + 1; ++n) {
                    r_compiled += "\t";
                }
                r_compiled += "pass\n";
                ++statements;
            }
        } else if (c == ';' || c == '\n') {
            if (!emit_statement(segment, depth, false, r_compiled, r_error, statements, repeat_counter)) {
                return false;
            }
            segment = String();
        } else {
            segment += String::chr(c);
        }
    }
    if (in_string) {
        r_error = "Unterminated string in Noqeri scene script.";
        return false;
    }
    if (depth != 0) {
        r_error = "Unclosed '{' in Noqeri scene script.";
        return false;
    }
    return emit_statement(segment, depth, false, r_compiled, r_error, statements, repeat_counter);
}
