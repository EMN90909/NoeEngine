@tool
extends EditorImportPlugin

# Copyright (c) 2026 Noethric.
# SPDX-License-Identifier: LicenseRef-Noethric-Proprietary
# Importing never compiles or executes user-controlled source files.
const SourceResource = preload("res://addons/noeengine_noqeri/noqeri_source.gd")

func _get_importer_name() -> String:
    return "noethric.noqeri.source"

func _get_visible_name() -> String:
    return "Noqeri source (.nqr)"

func _get_recognized_extensions() -> PackedStringArray:
    return PackedStringArray(["nqr"])

func _get_save_extension() -> String:
    return "res"

func _get_resource_type() -> String:
    return "Resource"

func _get_preset_count() -> int:
    return 0

func _get_import_options(_path: String, _preset_index: int) -> Array[Dictionary]:
    return []

func _get_option_visibility(_path: String, _option_name: StringName, _options: Dictionary) -> bool:
    return true

func _import(source_file: String, save_path: String, _options: Dictionary, _platform_variants: Array[String], _gen_files: Array[String]) -> Error:
    var file := FileAccess.open(source_file, FileAccess.READ)
    if file == null:
        return ERR_FILE_CANT_OPEN
    var resource = SourceResource.new()
    resource.source_path = source_file
    resource.source_code = file.get_as_text()
    file.close()
    return ResourceSaver.save(resource, save_path + ".res")
