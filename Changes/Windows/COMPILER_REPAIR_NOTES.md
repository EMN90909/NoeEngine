# NoeEngine Windows GCC repair notes

The expected UCRT64 compiler folder contains gcc.exe, g++.exe, windres.exe,
x86_64-w64-mingw32-gcc.exe, and x86_64-w64-mingw32-g++.exe.
The bootstrap now verifies and prints the exact missing executable.
If one is absent, it uses a targeted pacman reinstall rather than the
old --needed command, which would skip an already registered but damaged
compiler package.

The MSYS2 local database backup-first health check is retained. Its failure
stops installation and preserves diagnostics. No upstream Godot MIT material
is re-licensed; NOETHRIC-PROPRIETARY-LICENSE.txt applies to original
Noethric-owned contributions only.
