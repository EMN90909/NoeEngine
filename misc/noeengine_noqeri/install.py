#!/usr/bin/env python3
"""Install the optional NoeEngine Noqeri editor addon into a Godot-compatible project.

No compiler is downloaded, installed, or executed. Existing addon files are
never overwritten. The user explicitly enables the plugin in Project Settings.
"""
# Copyright (c) 2026 Noethric.
# SPDX-License-Identifier: LicenseRef-Noethric-Proprietary
import argparse
from pathlib import Path
import shutil
import sys

HERE = Path(__file__).resolve().parent
SOURCE = HERE / "addons" / "noeengine_noqeri"

def install(project_dir: Path) -> Path:
    project_dir = project_dir.expanduser().resolve()
    if not (project_dir / "project.godot").is_file():
        raise ValueError("Project directory must contain project.godot: " + str(project_dir))
    if not SOURCE.is_dir() or not (SOURCE / "plugin.cfg").is_file():
        raise ValueError("Noqeri addon files are missing: " + str(SOURCE))
    destination = project_dir / "addons" / "noeengine_noqeri"
    if destination.exists():
        raise FileExistsError("Addon already exists. Refusing to overwrite: " + str(destination))
    destination.parent.mkdir(parents=True, exist_ok=True)
    shutil.copytree(SOURCE, destination, symlinks=False)
    return destination

def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("project", type=Path, help="Path to a Godot/NoeEngine project containing project.godot")
    args = parser.parse_args(argv)
    try:
        destination = install(args.project)
    except (OSError, ValueError) as error:
        parser.exit(1, "Noqeri addon installation failed: " + str(error) + "\n")
    print("Installed the Noqeri editor addon to:", destination)
    print("In NoeEngine: Project -> Project Settings -> Plugins -> enable 'NoeEngine Noqeri'.")
    print("Select a trusted standalone Noqeri compiler via Project -> Tools -> Noqeri: Select compiler.")
    return 0

if __name__ == "__main__":
    sys.exit(main())
