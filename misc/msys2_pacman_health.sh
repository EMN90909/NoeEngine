#!/usr/bin/env bash
# Copyright (c) 2026 Noethric.
# SPDX-License-Identifier: LicenseRef-Noethric-Proprietary
# Guard and repair damaged MSYS2 pacman metadata without deleting records.
set -Eeuo pipefail
mode="$1"
if [[ "$mode" != "--check" && "$mode" != "--repair" ]]; then
  echo "[NoeEngine] Usage: msys2_pacman_health.sh --check|--repair" >&2
  exit 2
fi
local_db="${NOE_PACMAN_LOCAL_DB:-/var/lib/pacman/local}"
cache="${NOE_PACMAN_CACHE:-/var/cache/pacman/pkg}"
pacman_bin="${NOE_PACMAN_BIN:-pacman}"
if [[ ! -d "$local_db" ]]; then
  echo "[NoeEngine] Pacman local database does not exist: $local_db" >&2
  exit 2
fi
shopt -s nullglob
count=0
for dir in "$local_db"/*/; do
  if [[ ! -s "$dir/desc" ]]; then
    count=$((count + 1))
    echo "[NoeEngine] Broken Pacman record: $(basename "$dir")" >&2
  fi
done
if (( count == 0 )); then
  echo "[NoeEngine] Pacman local package metadata: OK"
  exit 0
fi
if [[ "$mode" == "--check" ]]; then
  echo "[NoeEngine] Refusing upgrade with $count incomplete records; run build.bat to attempt a backed-up repair." >&2
  exit 3
fi
# Back up installed package metadata before any repair. Do not remove records
# or write new metadata unless the exact cached package is available.
stamp="$(date -u +%Y%m%dT%H%M%SZ)-$$"
backup="$local_db-noe-backup-$stamp"
cp -a -- "$local_db" "$backup"
echo "[NoeEngine] Pacman metadata backup: $backup"
for dir in "$local_db"/*/; do
  if [[ -s "$dir/desc" ]]; then continue; fi
  name="$(basename "$dir")"
  set -- "$cache/$name"-*.pkg.tar.zst "$cache/$name"-*.pkg.tar.xz "$cache/$name"-*.pkg.tar.gz
  if (( $# == 0 )); then
    echo "[NoeEngine] Exact signed package archive for $name missing from $cache" >&2
    echo "[NoeEngine] Database backup: $backup. Restore from a trusted archive or reinstall MSYS2 after preserving your installed packages; do not delete the Pacman database." >&2
    exit 4
  fi
  archive="$1"
  echo "[NoeEngine] Restoring metadata for $name from $archive"
  if ! "$pacman_bin" -U --dbonly --noconfirm -- "$archive"; then
    echo "[NoeEngine] Pacman rejected metadata repair for $name; backup is $backup" >&2
    exit 5
  fi
  if [[ ! -s "$dir/desc" ]]; then
    echo "[NoeEngine] $name still incomplete after --dbonly; backup is $backup" >&2
    exit 6
  fi
done
echo "[NoeEngine] Pacman metadata repair completed; original backup: $backup"
