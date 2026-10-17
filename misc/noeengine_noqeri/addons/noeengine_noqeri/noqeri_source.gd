@tool
extends Resource
class_name NoeNoqeriSource

# Copyright (c) 2026 Noethric.
# SPDX-License-Identifier: LicenseRef-Noethric-Proprietary
# A Godot Resource wrapper for imported .nqr source, not an executable Script.
@export_file("*.nqr") var source_path: String = ""
@export_multiline var source_code: String = ""
