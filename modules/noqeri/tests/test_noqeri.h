// Copyright (c) 2026 Noethric.
// SPDX-License-Identifier: LicenseRef-Noethric-Proprietary
#ifndef TEST_NOEENGINE_NOQERI_H
#define TEST_NOEENGINE_NOQERI_H

#include "../noqeri_translator.h"
#include "tests/test_macros.h"

namespace TestNoeEngineNoqeri {

TEST_CASE("[Noqeri] Native .nqr function translation") {
    String compiled;
    String error;
    CHECK(noqeri_translate_game_script(
            "function _ready(): void {\n"
            "  let count: int = 2\n"
            "  if count > 1 { print(\"Noqeri ready\") }\n"
            "}\n",
            compiled, error));
    CHECK(error.is_empty());
    CHECK(compiled.contains("extends Node"));
    CHECK(compiled.contains("func _ready() -> void:"));
    CHECK(compiled.contains("var count: int = 2"));
    CHECK(compiled.contains("if count > 1:"));
    CHECK(compiled.contains("print(\"Noqeri ready\")"));
}

TEST_CASE("[Noqeri] Native scene base directive") {
    String compiled;
    String error;
    CHECK(noqeri_translate_game_script(
            "// noeengine:extends Node2D\nfunction _process(delta: float): void {\n}\n",
            compiled, error));
    CHECK(compiled.begins_with("extends Node2D\n"));
    CHECK(compiled.contains("func _process(delta: float) -> void:"));
    CHECK(compiled.contains("pass\n"));
}

TEST_CASE("[Noqeri] Unsupported systems features fail closed") {
    String compiled;
    String error;
    CHECK_FALSE(noqeri_translate_game_script(
            "import \"std/file.nqr\"\nfunction _ready(): void {}",
            compiled, error));
    CHECK(error.contains("requires the native Noqeri runtime"));
}

TEST_CASE("[Noqeri] No unsupported blocks or malformed braces") {
    String compiled;
    String error;
    CHECK_FALSE(noqeri_translate_game_script(
            "record Point { x: int }", compiled, error));
    CHECK_FALSE(noqeri_translate_game_script(
            "function _ready() {", compiled, error));
    CHECK_FALSE(noqeri_translate_game_script(
            "function _ready() { } }", compiled, error));
}

TEST_CASE("[Noqeri] Comment-only game blocks emit a pass") {
    String compiled;
    String error;
    CHECK(noqeri_translate_game_script(
            "function _ready(): void { // no code yet\n}\n",
            compiled, error));
    CHECK(error.is_empty());
    CHECK(compiled.contains("\tpass\n"));
}

TEST_CASE("[Noqeri] Native bridge rejects incompatible type casts") {
    String compiled;
    String error;
    CHECK_FALSE(noqeri_translate_game_script(
            "function _ready(): void { let number = 2 as u32 }",
            compiled, error));
    CHECK(error.contains("cast semantics"));
}

TEST_CASE("[Noqeri] repeat evaluates a count once with zero floor") {
    String compiled;
    String error;
    CHECK(noqeri_translate_game_script(
            "function _ready(): void {\n"
            "    let total: int = 0\n"
            "    repeat 4 { total = total + 3 }\n"
            "    print(total)\n"
            "}\n",
            compiled, error));
    CHECK(error.is_empty());
    CHECK(compiled.contains("for __nqr_repeat_0 in range(max(0, int(4))):"));
    CHECK(compiled.contains("total = total + 3"));
}

TEST_CASE("[Noqeri] repeat is invalid without braces") {
    String compiled;
    String error;
    CHECK_FALSE(noqeri_translate_game_script(
            "function _ready(): void {\nrepeat 3\nprint(1)\n}\n",
            compiled, error));
    CHECK(error.contains("requires a"));
}

TEST_CASE("[Noqeri] repeated counted loops use separate counters") {
    String compiled;
    String error;
    CHECK(noqeri_translate_game_script(
            "function _ready(): void {\n"
            "repeat 2 { print(1) }\n"
            "repeat 3 { print(2) }\n"
            "}\n",
            compiled, error));
    CHECK(compiled.contains("__nqr_repeat_0"));
    CHECK(compiled.contains("__nqr_repeat_1"));
}

TEST_CASE("[Noqeri] Scene scripts reject free-standing execution") {
    String compiled;
    String error;
    CHECK_FALSE(noqeri_translate_game_script(
            "print(\"only allowed inside _ready\")\n", compiled, error));
    CHECK(error.contains("Top-level Noqeri scene script"));
}

TEST_CASE("[Noqeri] Scene scripts reject native GDScript-only annotations") {
    String compiled;
    String error;
    CHECK_FALSE(noqeri_translate_game_script(
            "@tool\nfunction _ready(): void {}\n", compiled, error));
    CHECK(error.contains("Top-level Noqeri scene script"));
}

} // namespace TestNoeEngineNoqeri
#endif
