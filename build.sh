#!/usr/bin/env bash
# Native installer build entry point for NoeEngine by Noethric.
set -Eeuo pipefail
cd "$(dirname "${BASH_SOURCE[0]}")"
mkdir -p Products

usage() {
  cat <<'USAGE'
Usage: ./build.sh [linux|windows|macos|all] [jobs]
  linux    Build Debian .deb on Linux (default)
  windows  Build Windows .exe setup (Linux + MinGW-w64 + NSIS, or Windows)
  macos    Build macOS .pkg on a Mac with Xcode and macOS SDK
  all      Linux: build Linux and Windows; Mac: build macOS
           macOS cannot be produced with an ordinary Debian toolchain.
  jobs     Number of parallel compiler jobs (default: 2)
Output filenames: Products/NoeEngine-setup.{deb,exe,pkg}
USAGE
}

target="${1:-linux}"
jobs="${2:-${JOBS:-2}}"
if [[ "$target" == "-h" || "$target" == "--help" ]]; then usage; exit 0; fi
if [[ ! "$jobs" =~ ^[1-9][0-9]*$ ]]; then echo "Invalid job count" >&2; exit 2; fi
if ! command -v python3 >/dev/null; then echo "Python 3 is required" >&2; exit 2; fi

build_one() {
  echo "Building $1 setup installer into Products/..."
  python3 misc/build_products.py "$1" --jobs "$jobs"
}

case "$target" in
  linux|windows|macos) build_one "$target" ;;
  all)
    case "$(uname -s)" in
      Linux) build_one linux; build_one windows ;;
      Darwin) build_one macos ;;
      *) echo "Use Linux or macOS for ./build.sh all" >&2; exit 2 ;;
    esac ;;
  *) usage >&2; exit 2 ;;
esac

printf '\nSetup installers created:\n'
find Products -maxdepth 1 -type f -name 'NoeEngine-setup.*' -print
