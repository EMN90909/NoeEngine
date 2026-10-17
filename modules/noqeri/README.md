# Noqeri in NoeEngine's native Script editor

NoeEngine 0.1.0-dev registers **Noqeri** as a native Godot `ScriptLanguage`
and **NoqeriScript** as a `.nqr` Script resource. No separate Noqeri dock
or plugin is needed. The `.nqr` file opens in the same Script editor as
`.gd`, and a supported `.nqr` script can be assigned to a Node.

## Quick start

1. Compile NoeEngine with both built-in scripting modules enabled
   (`module_gdscript_enabled=yes module_noqeri_enabled=yes`, which are
   enabled by default).
2. Create a new `.nqr` file with the following contents and attach it
   to a `Node2D` in your scene:

```noqeri
// noeengine:extends Node2D
function _ready(): void {
    print("Hello from native Noqeri in NoeEngine")
}

function _process(delta: float): void {
    if delta > 0 {
        let velocity: float = 12.0
        position.x = position.x + velocity * delta
    }
}
```

3. Save in the existing Script editor. Run the scene as normal.

Godot's native inspector can attach the source because the resource loader
reports the type `NoqeriScript`, which derives from `Script`.

## Language subset and execution

Implemented subset: `function` declarations (including `_ready` and
`_process`), `let` variables, `const`, `return`, `if`/`else`,
`while`, primitive `int`/`float`/`bool`/`string` type declarations,
string literals, basic arithmetic, and calls/properties supported by the
engine's existing GDScript VM.

Optional `// noeengine:extends Node2D` comment chooses an engine Node base
from a small allowlist. It does not affect the standalone Noqeri compiler.

**Not supported yet:** `import`, `module`, `record`, `export`,
`extern`, `unsafe`, `try`/`throw`, `repeat`, fixed-width integer
semantics, Noqeri atomics/FFI/pointers/stdlib, native NIR execution, and
complete Noqeri type checking. Unsupported syntax returns a clear
error instead of being passed through as if it were fully compatible.
GDScript VM behavior may not match complete Noqeri language semantics.

The Noethric C++ module implements script registration and translation.
Noqeri compiler code is **GPL-3.0-only** and is not shipped or linked.

## Tests

The source includes `modules/noqeri/tests/test_noqeri.h` with C++ unit
tests and `tests/noqeri_scene` with a minimal Node scene.
After building NoeEngine, run:

```sh
scons platform=linuxbsd target=editor tests=yes -j2
bin/godot.linuxbsd.editor.* --headless --test
bin/godot.linuxbsd.editor.* --headless --path tests/noqeri_scene --quit
```

The scene test prints `NOQERI_NATIVE_READY` and quits. Do not claim these
tests passed without compiling and executing the engine.

## Compatibility and licensing

NoeEngine is a product of Noethric derived from Godot 4.0.5 RC (MIT).
The new native Noqeri module is original proprietary Noethric code.
Noqeri's separate full compiler is licensed GPL-3.0-only.
The native scene bridge does not implement its complete runtime or ABI.
See `NOTICE.md` and `LICENSE.md` at the repository root.
