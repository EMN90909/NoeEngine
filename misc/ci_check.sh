#!/usr/bin/env bash
# Run local CI preflight before spending time on a full C++ compile.
set -Eeuo pipefail
cd "$(dirname "${BASH_SOURCE[0]}")/.."
echo "[CI] Shell syntax"
bash -n build.sh
bash -n misc/ci_check.sh
echo "[CI] Python syntax"
python3 -m py_compile version.py misc/build_products.py
echo "[CI] Installer packaging tests"
python3 -m unittest discover -s tests/python_build -p 'test_no*.py' -v
if command -v actionlint >/dev/null 2>&1; then
  echo "[CI] GitHub workflow validation"
  actionlint .github/workflows/*.yml
else
  echo "[CI] actionlint unavailable; workflow-schema lint skipped"
fi
echo "[CI] Local preflight PASS (this does not compile the C++ engine)"
