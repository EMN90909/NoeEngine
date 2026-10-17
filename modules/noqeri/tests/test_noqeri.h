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

} // namespace TestNoeEngineNoqeri
#endif
