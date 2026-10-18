#!/usr/bin/env bash
# Copyright (c) 2026 Noethric.
# SPDX-License-Identifier: LicenseRef-Noethric-Proprietary
# Build native NoeEngine app releases, do not fabricate cross-platform binaries.
set -Eeuo pipefail
ROOT="$(CDPATH= cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
cd "$ROOT"
mkdir -p Products

usage() {
  cat <<'USAGE'
Usage: ./build.sh [linux|windows|macos|android|all|check|patch-tools] [jobs]
                  [--install-deps] [--tests] [--package-only]

  ./build.sh linux 2                 Linux runnable AppDir and .tar.gz (+ .deb if available)
  ./build.sh windows 2               Windows .exe setup using MinGW-w64 and NSIS
  ./build.sh macos 2                 macOS .pkg setup using Xcode on a Mac
  ./build.sh android 2               Actual Android arm64 editor .apk, via NDK + Gradle
  ./build.sh all 2                   Available targets on this host; no fake builds
  ./build.sh android 2 --install-deps
                                    Install supported OS packages + verified Android SDK
  ./build.sh check                   Offline preflight, Python tests, no C++ compilation
  ./build.sh patch-tools             Build and test local signed patch tools
  ./build.sh linux 2 --tests         Compile editor + native Noqeri tests

  --install-deps explicitly authorizes system package downloads and installs.
  Android SDK license acceptance is NOT automatic; sdkmanager prompts as needed.
  macOS requires Xcode and Apple SDK: cannot be downloaded on Linux by this script.
  Linux AppDir runs in place; the .tar.gz is a portable archive, not an AppImage.
  Android generates a mobile editor APK (not a desktop GUI running on Android).
  Results and build logs are placed in Products/.
USAGE
}

target="${1:-linux}"
if [[ "$target" == -h || "$target" == --help ]]; then usage; exit 0; fi
case "$target" in linux|windows|macos|android|all|check|patch-tools) ;; *) usage >&2; exit 2 ;; esac
if [[ $# -gt 0 ]]; then shift; fi
jobs="${JOBS:-2}"
if [[ $# -gt 0 && "$1" =~ ^[0-9]+$ ]]; then jobs="$1"; shift; fi
[[ "$jobs" =~ ^[1-9][0-9]*$ ]] || { echo "jobs must be positive" >&2; exit 2; }
with_tests=0
package_only=0
install_deps=0
while [[ $# -gt 0 ]]; do
  case "$1" in
    --tests) with_tests=1 ;;
    --package-only) package_only=1 ;;
    --install-deps) install_deps=1 ;;
    -h|--help) usage; exit 0 ;;
    *) echo "Unknown argument: $1" >&2; exit 2 ;;
  esac
  shift
done

if [[ "$target" == patch-tools ]]; then
  command -v go >/dev/null || { echo "Go 1.23 or newer is required" >&2; exit 2; }
  for tool in noe-updater noe-patch-sign; do
    (cd "tools/$tool" && GOTOOLCHAIN=local GOPROXY=off go test ./...)
    (cd "tools/$tool" && GOTOOLCHAIN=local GOPROXY=off go build -o "$ROOT/Products/$tool" .)
  done
  echo "Patch tools created in Products/"
  exit 0
fi
command -v python3 >/dev/null || { echo "Python 3 is required" >&2; exit 2; }
if [[ ! -f SConstruct || ! -f modules/noqeri/noqeri_script.cpp || ! -f LICENSE.txt ]]; then
  echo "Run from a complete NoeEngine checkout" >&2
  exit 2
fi
if [[ "$package_only" == 1 && ( "$target" == check || "$with_tests" == 1 ) ]]; then
  echo "package-only cannot run checks/native tests" >&2; exit 2
fi
if [[ "$with_tests" == 1 && "$target" != linux ]]; then
  echo "Native unit tests require the Linux target" >&2; exit 2
fi

case "$(uname -s):$target" in
  Linux:all) targets=(linux windows android) ;;
  Darwin:all) targets=(macos android) ;;
  MINGW*:all|MSYS*:all|CYGWIN*:all) targets=(windows) ;;
  *) targets=("$target") ;;
esac
if [[ "$install_deps" == 1 ]]; then
  echo "[NoeEngine] Installing requested toolchains (network and privileged packages)"
  python3 misc/bootstrap_build_deps.py "${targets[@]}" --install-deps
fi
if [[ "$target" == android || "$target" == all ]]; then
  sdk_default="$HOME/.local/share/noeengine/android-sdk"
  if [[ -z "${ANDROID_HOME:-}" && -z "${ANDROID_SDK_ROOT:-}" ]]; then
    export ANDROID_HOME="$sdk_default"
  elif [[ -z "${ANDROID_HOME:-}" ]]; then
    export ANDROID_HOME="$ANDROID_SDK_ROOT"
  fi
fi

echo "[NoeEngine] Preparing branding"
python3 misc/apply_noeengine_branding.py
if [[ "$target" == check || "$package_only" == 0 ]]; then
  bash misc/ci_check.sh 2>&1 | tee Products/preflight.log
fi
if [[ "$target" == check ]]; then
  echo "Static/preflight checks passed; editor C++ was not compiled"
  exit 0
fi

# Refuse to start expensive compilation unless all host SDKs are present.
for platform in "${targets[@]}"; do
  python3 misc/build_products.py "$platform" --preflight
done
for platform in "${targets[@]}"; do
  args=("$platform" --jobs "$jobs")
  [[ "$with_tests" == 1 ]] && args+=(--tests)
  [[ "$package_only" == 1 ]] && args+=(--package-only)
  echo "[NoeEngine] Building $platform"
  python3 misc/build_products.py "${args[@]}" 2>&1 | tee "Products/build-$platform.log"
  case "$platform" in
    linux) artifact="Products/NoeEngine-linux-x86_64.tar.gz" ;;
    windows) artifact="Products/NoeEngine-setup.exe" ;;
    macos) artifact="Products/NoeEngine-setup.pkg" ;;
    android) artifact="Products/NoeEngine-android-arm64.apk" ;;
  esac
  if [[ ! -s "$artifact" ]]; then
    echo "Build did not generate expected artifact: $artifact" >&2; exit 1
  fi
  printf 'Generated %s (%s bytes)\n' "$artifact" "$(wc -c < "$artifact")"
done
echo "[NoeEngine] Completed available targets. Artifacts: Products/"
