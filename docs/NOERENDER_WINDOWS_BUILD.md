# Windows build from source

Run `build.bat 2` from Windows CMD or double-click `build.bat` in the
repository root. It calls the checked-in `misc/setup_windows_build.ps1`:

1. Uses Windows Package Manager (WinGet) from Microsoft App Installer to install
   Python 3.12, MSVC Build Tools 2022 with the C++ tools and Windows SDK, and
   NSIS when they are absent. Package manager prompts/elevation may be needed.
2. Installs a pinned SCons 4.8.1 with Python's pip if missing.
3. Validates native compilation and packaging requirements, then calls the
   existing `misc/build_products.py windows --jobs N` entry point.
4. Checks `Products/NoeEngine-setup.exe` exists and is nonempty.

Use `powershell.exe -NoProfile -ExecutionPolicy Bypass -File
misc/setup_windows_build.ps1 -CheckOnly -SkipInstall` to verify existing
dependencies without downloading software or building.

The script never downloads a random remote PowerShell script. Installations
come from WinGet's signed manifests and Microsoft's VS installer. It fails
rather than concealing missing SDKs or generating a fake installer. It does
not install Vulkan runtime libraries for redistribution: the engine compiles
with its checked-in Vulkan headers and loader integration.

A reboot or shell restart may be necessary after initial VS installation.
Windows builds require substantial free disk space and may run for a while.
CI Windows runners have the required SDK preinstalled and can use
`misc/build_products.py` directly.
