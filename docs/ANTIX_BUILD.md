# Compiling NoeEngine on antiX 26 / Debian 13

## Why the previous build stopped

On antiX 26, the `libudev-dev` Debian compatibility package can conflict with antiX `libeudev-dev` versions. Previously, NoeEngine's system-package bootstrap always requested `libudev-dev` on Linux. That stops `apt-get` with dependency-solver error 100 **before SCons starts compiling C++**.

`misc/bootstrap_build_deps.py` now identifies antiX via `/etc/os-release` and does **not** request either `libudev-dev` or `libeudev-dev`. The regular X11, Mesa, ALSA, PulseAudio, Python, SCons and compiler dependencies remain installed. This is deliberate: `platform/linuxbsd/detect.py` uses `pkg-config --exists libudev` and disables optional controller hotplug support if the development library is missing. It does not prevent the editor from compiling.

**Do not** use `apt --fix-broken install`, force package downgrades, remove system eudev packages, or overwrite system `libudev.so` merely to build the engine.

## Recommended Linux-only build

On the user's low-memory antiX laptop, **do not** start with bare `./build.sh`; by design that command also attempts Windows and Android toolchains after Linux.

Use a **fresh checkout from main**. A GitHub ZIP extracted to `NoeEngine-main` is not a Git repository, so `git pull` will not update it. Either download a new ZIP or clone the repository.

```bash
git clone https://github.com/EMN90909/NoeEngine.git
cd NoeEngine
chmod +x build.sh

# No privileged packages are installed during these checks.
./build.sh check
python3 misc/bootstrap_build_deps.py linux --install-deps --dry-run

# Install only Linux build dependencies, then compile using one job.
./build.sh linux 1 --install-deps
```

To compile native Noqeri tests too, run `./build.sh linux 1 --tests` once dependencies are installed. Results go to `Products/build-linux.log`, while `Products/preflight.log` contains the Python preflight.

To inspect antiX support without changing the system:

```bash
cat /etc/os-release
pkg-config --exists libudev && echo 'udev headers found' || echo 'udev disabled; optional controller hotplug only'
python3 -m unittest discover -s tests/python_build -p 'test_no*.py' -v
```

## Other platforms

The no-argument script intentionally runs preflight, Linux, Windows and Android build stages. Windows needs MinGW-w64/NSIS, while Android needs JDK, an SDK/NDK, Gradle and accepted SDK licenses. Linux compilation passing does not imply the other platform builds passed, and an Android build can require significant additional disk and memory.

## Validation status

The user-provided log of the **previous** checkout shows all 53 preflight tests passing before the antiX `apt-get` conflict. New antiX bootstrap tests were added to `tests/python_build/test_noe_bootstrap_deps.py`. The actual engine C++ compiler and the updated build have **not** yet been run on the user's antiX machine. Re-run the commands above on the latest commit to verify the real compiler.
