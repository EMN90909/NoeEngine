#!/usr/bin/env bash
# Copyright (c) 2026 Noethric.
# SPDX-License-Identifier: LicenseRef-Noethric-Proprietary
# Build the editor and native setup installer in Products/.
set -Eeuo pipefail
ROOT="$(CDPATH= cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
cd "$ROOT"
mkdir -p Products

usage() {
  cat <<'USAGE'
Usage: ./build.sh [linux|windows|macos|all|check|patch-tools] [jobs] [--tests] [--package-only]

  ./build.sh                      Build and smoke-test Linux editor, make .deb
  ./build.sh linux 2              Debian/antiX -> Products/NoeEngine-setup.deb
  ./build.sh windows 2            MinGW-w64 + NSIS -> Products/NoeEngine-setup.exe
  ./build.sh macos 2              macOS + Xcode -> Products/NoeEngine-setup.pkg
  ./build.sh all 2                Linux: Linux+Windows; macOS: macOS
  ./build.sh check                Local checks only; no C++ compilation
  ./build.sh patch-tools          Test/build Go signed patch installer and offline signer
  ./build.sh linux 2 --tests      Build and run Noqeri native C++ tests
  ./build.sh linux 2 --package-only Package a previously built editor

  Two compiler jobs is the low-RAM default. Logs and installers: Products/.
  Required toolchains: BUILD_PRODUCTS.md. Linux cannot natively build macOS.
USAGE
}
target="${1:-linux}"
if [[ "$target" == "-h" || "$target" == "--help" ]]; then usage; exit 0; fi
case "$target" in linux|windows|macos|all|check|patch-tools) ;; *) usage >&2; exit 2 ;; esac
if [[ "$target" == patch-tools ]]; then
  command -v go >/dev/null 2>&1 || { echo "ERROR: Go 1.23+ is required for patch-tools." >&2; exit 2; }
  for tool in noe-updater noe-patch-sign; do
    echo "[NoeEngine] Testing tools/$tool"
    (cd "tools/$tool" && GOTOOLCHAIN=local GOPROXY=off go test ./...)
    (cd "tools/$tool" && GOTOOLCHAIN=local GOPROXY=off go build -o "$ROOT/Products/$tool" .)
  done
  echo "[NoeEngine] Patch binaries saved to Products/ (signing key never stored here)."
  exit 0
fi
if [[ "$#" -gt 0 ]]; then shift; fi
jobs="${JOBS:-2}"
if [[ "$#" -gt 0 && "$1" =~ ^[0-9]+$ ]]; then jobs="$1"; shift; fi
[[ "$jobs" =~ ^[1-9][0-9]*$ ]] || { echo "ERROR: jobs must be positive" >&2; exit 2; }
with_tests=0
package_only=0
while [[ "$#" -gt 0 ]]; do
  case "$1" in
    --tests) with_tests=1 ;;
    --package-only) package_only=1 ;;
    -h|--help) usage; exit 0 ;;
    *) echo "Unknown option: $1" >&2; usage >&2; exit 2 ;;
  esac
  shift
done
command -v python3 >/dev/null 2>&1 || { echo "ERROR: Python 3 required." >&2; exit 2; }
if [[ ! -f SConstruct || ! -f modules/noqeri/noqeri_script.cpp || ! -f LICENSE.txt ]]; then
  echo "ERROR: Run from a complete NoeEngine source checkout." >&2
  exit 2
fi
if [[ "$target" == check && "$package_only" == 1 ]]; then
  echo "ERROR: --package-only does not apply to check." >&2
  exit 2
fi
if [[ "$with_tests" == 1 && "$package_only" == 1 ]]; then
  echo "ERROR: --tests requires a fresh compile." >&2
  exit 2
fi
echo "[NoeEngine] Applying app branding and dark editor palette"
python3 misc/apply_noeengine_branding.py
if [[ "$target" == check || "$package_only" == 0 ]]; then
  echo "[NoeEngine] Running offline preflight (Products/preflight.log)"
  bash misc/ci_check.sh 2>&1 | tee Products/preflight.log
fi
if [[ "$target" == check ]]; then
  echo "[NoeEngine] Preflight passed; no C++ compilation requested."
  exit 0
fi
if [[ "$with_tests" == 1 && "$target" != linux && "$target" != all ]]; then
  echo "ERROR: --tests is supported for native Linux builds only." >&2
  exit 2
fi
case "$target" in
  all)
    case "$(uname -s)" in
      Linux) targets=(linux windows) ;;
      Darwin) targets=(macos) ;;
      *) echo "ERROR: all requires Linux or macOS." >&2; exit 2 ;;
    esac
    ;;
  *) targets=("$target") ;;
esac
if [[ "$with_tests" == 1 && "${targets[0]}" != linux ]]; then
  echo "ERROR: --tests requires a native Linux target." >&2
  exit 2
fi

# Check all target toolchains *before* beginning any lengthy compilation.
for platform in "${targets[@]}"; do
  python3 misc/build_products.py "$platform" --preflight
done
for platform in "${targets[@]}"; do
  args=("$platform" --jobs "$jobs")
  if [[ "$with_tests" == 1 && "$platform" == linux ]]; then args+=(--tests); fi
  if [[ "$package_only" == 1 ]]; then args+=(--package-only); fi
  case "$platform" in linux) suffix=deb ;; windows) suffix=exe ;; macos) suffix=pkg ;; esac
  output="Products/NoeEngine-setup.$suffix"
  rm -f -- "$output"
  echo "[NoeEngine] Building $platform (Products/build-$platform.log)"
  python3 misc/build_products.py "${args[@]}" 2>&1 | tee "Products/build-$platform.log"
  if [[ ! -s "$output" ]]; then
    echo "ERROR: Setup installer missing or empty: $output" >&2
    exit 1
  fi
  echo "[NoeEngine] Created $output"
done
printf '\n[NoeEngine] Setup installers:\n'
for platform in "${targets[@]}"; do
  case "$platform" in linux) suffix=deb ;; windows) suffix=exe ;; macos) suffix=pkg ;; esac
  ls -lh -- "Products/NoeEngine-setup.$suffix"
done
