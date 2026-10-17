@tool
extends EditorPlugin

# Copyright (c) 2026 Noethric.
# SPDX-License-Identifier: LicenseRef-Noethric-Proprietary
#
# Optional Noqeri editor and compiler CLI bridge. Noqeri is GPL-3.0-only
# and intentionally is NOT linked or included with this proprietary addon.
# This is NOT a Godot ScriptLanguage implementation or a scene runtime.
const IMPORTER = preload("res://addons/noeengine_noqeri/noqeri_importer.gd")
const SETTINGS_KEY := "noeengine/noqeri/compiler_path"
const KEYWORDS := [
    "let", "const", "function", "return", "if", "else", "while",
    "repeat", "record", "module", "import", "export", "extern",
    "true", "false", "null", "unsafe", "try", "throw", "as",
    "int", "float", "string", "bool", "void", "u8", "u16",
    "u32", "u64", "i8", "i16", "i32", "i64", "usize", "isize",
    "volatile", "atomic"
]

var importer: EditorImportPlugin
var panel: VBoxContainer
var editor: CodeEdit
var output: RichTextLabel
var source_label: Label
var compiler_label: Label
var open_dialog: EditorFileDialog
var save_dialog: EditorFileDialog
var compiler_dialog: EditorFileDialog
var run_confirmation: ConfirmationDialog
var current_path := ""
var compiler_path := ""
var dirty := false

func _enter_tree() -> void:
    importer = IMPORTER.new()
    add_import_plugin(importer)
    _create_panel()
    add_control_to_bottom_panel(panel, "Noqeri")
    _create_dialogs()
    add_tool_menu_item("Noqeri: Edit selected .nqr", _edit_selection)
    add_tool_menu_item("Noqeri: Check .nqr", _check)
    add_tool_menu_item("Noqeri: Select compiler", _select_compiler)
    var settings := EditorInterface.get_editor_settings()
    if settings.has_setting(SETTINGS_KEY):
        compiler_path = str(settings.get_setting(SETTINGS_KEY))
    elif OS.has_environment("NOQERI_BIN"):
        compiler_path = OS.get_environment("NOQERI_BIN")
    _refresh_labels()
    _set_output("Noqeri source editor is ready. Select a trusted Noqeri compiler to enable checks.\nNoqeri scripts are not native scene scripts yet.")

func _exit_tree() -> void:
    remove_tool_menu_item("Noqeri: Edit selected .nqr")
    remove_tool_menu_item("Noqeri: Check .nqr")
    remove_tool_menu_item("Noqeri: Select compiler")
    if panel != null:
        remove_control_from_bottom_panel(panel)
        panel.queue_free()
    for dialog in [open_dialog, save_dialog, compiler_dialog, run_confirmation]:
        if dialog != null:
            dialog.queue_free()
    if importer != null:
        remove_import_plugin(importer)
        importer = null

func _create_panel() -> void:
    panel = VBoxContainer.new()
    panel.name = "Noqeri"
    panel.custom_minimum_size = Vector2(380, 250)
    var header := HBoxContainer.new()
    panel.add_child(header)
    source_label = Label.new()
    source_label.size_flags_horizontal = Control.SIZE_EXPAND_FILL
    source_label.clip_text = true
    header.add_child(source_label)
    _button(header, "New", "Start a new .nqr file", _new_source)
    _button(header, "Open", "Open selected or project .nqr source", _edit_selection)
    _button(header, "Save", "Save this .nqr source", _save)
    _button(header, "Check", "Run Noqeri's non-executing 'check' command", _check)
    _button(header, "Run*", "Experimental 'run' (legacy compiler only)", _confirm_run)
    editor = CodeEdit.new()
    editor.size_flags_vertical = Control.SIZE_EXPAND_FILL
    editor.custom_minimum_size.y = 140
    editor.gutters_draw_line_numbers = true
    editor.indent_automatic = true
    var highlighter := CodeHighlighter.new()
    for keyword in KEYWORDS:
        highlighter.add_keyword_color(keyword, Color(0.55, 0.78, 1.0))
    highlighter.add_color_region('"', '"', Color(0.95, 0.70, 0.45))
    highlighter.add_color_region("//", "", Color(0.53, 0.65, 0.55), true)
    editor.syntax_highlighter = highlighter
    editor.text_changed.connect(_on_text_changed)
    panel.add_child(editor)
    var tools := HBoxContainer.new()
    panel.add_child(tools)
    compiler_label = Label.new()
    compiler_label.size_flags_horizontal = Control.SIZE_EXPAND_FILL
    compiler_label.clip_text = true
    tools.add_child(compiler_label)
    _button(tools, "Compiler...", "Select separately installed Noqeri executable", _select_compiler)
    output = RichTextLabel.new()
    output.bbcode_enabled = false
    output.scroll_following = true
    output.custom_minimum_size.y = 80
    panel.add_child(output)

func _button(parent: Node, label: String, tip: String, callback: Callable) -> void:
    var control := Button.new()
    control.text = label
    control.tooltip_text = tip
    control.custom_minimum_size.y = 28
    control.pressed.connect(callback)
    parent.add_child(control)

func _create_dialogs() -> void:
    open_dialog = EditorFileDialog.new()
    open_dialog.file_mode = EditorFileDialog.FILE_MODE_OPEN_FILE
    open_dialog.access = EditorFileDialog.ACCESS_RESOURCES
    open_dialog.add_filter("*.nqr", "Noqeri source")
    open_dialog.file_selected.connect(_load_source)
    add_child(open_dialog)

    save_dialog = EditorFileDialog.new()
    save_dialog.file_mode = EditorFileDialog.FILE_MODE_SAVE_FILE
    save_dialog.access = EditorFileDialog.ACCESS_RESOURCES
    save_dialog.add_filter("*.nqr", "Noqeri source")
    save_dialog.file_selected.connect(_write_source)
    add_child(save_dialog)

    compiler_dialog = EditorFileDialog.new()
    compiler_dialog.file_mode = EditorFileDialog.FILE_MODE_OPEN_FILE
    compiler_dialog.access = EditorFileDialog.ACCESS_FILESYSTEM
    compiler_dialog.file_selected.connect(_set_compiler)
    add_child(compiler_dialog)

    run_confirmation = ConfirmationDialog.new()
    run_confirmation.title = "Execute external Noqeri program?"
    run_confirmation.dialog_text = "Running code executes an external program with your user privileges.\nOnly use a trusted compiler and trusted .nqr file.\nThe current Noqeri Stage-2 CLI does not support 'run'."
    run_confirmation.confirmed.connect(_run_legacy)
    add_child(run_confirmation)

func _new_source() -> void:
    if dirty:
        _set_output("Save the current file before creating another one.")
        return
    current_path = ""
    editor.text = 'print("Hello from Noqeri")\n'
    dirty = true
    _refresh_labels()
    make_bottom_panel_item_visible(panel)

func _edit_selection() -> void:
    if dirty:
        _set_output("Save the current file before opening another one.")
        return
    for selected in EditorInterface.get_selected_paths():
        if str(selected).to_lower().ends_with(".nqr"):
            _load_source(str(selected))
            return
    open_dialog.popup_centered_ratio(0.75)

func _load_source(path: String) -> void:
    if dirty:
        _set_output("Save the current file before opening another one.")
        return
    if not path.begins_with("res://") or not path.to_lower().ends_with(".nqr"):
        _set_output("Only .nqr files inside this project can be opened.")
        return
    var file := FileAccess.open(path, FileAccess.READ)
    if file == null:
        _set_output("Cannot read: " + path)
        return
    editor.text = file.get_as_text()
    file.close()
    current_path = path
    dirty = false
    _refresh_labels()
    make_bottom_panel_item_visible(panel)
    _set_output("Opened " + path)

func _save() -> void:
    if current_path.is_empty():
        save_dialog.popup_centered_ratio(0.75)
    else:
        _write_source(current_path)

func _write_source(path: String) -> void:
    if not path.begins_with("res://") or not path.to_lower().ends_with(".nqr"):
        _set_output("Choose a .nqr file inside the current project.")
        return
    var file := FileAccess.open(path, FileAccess.WRITE)
    if file == null:
        _set_output("Cannot write: " + path)
        return
    file.store_string(editor.text)
    file.close()
    current_path = path
    dirty = false
    _refresh_labels()
    EditorInterface.get_resource_filesystem().scan()
    _set_output("Saved " + path)

func _on_text_changed() -> void:
    dirty = true
    _refresh_labels()

func _refresh_labels() -> void:
    if source_label != null:
        source_label.text = ("Unsaved .nqr" if current_path.is_empty() else current_path) + (" *" if dirty else "")
        source_label.tooltip_text = current_path
    if compiler_label != null:
        compiler_label.text = "Noqeri compiler: " + (compiler_path.get_file() if not compiler_path.is_empty() else "not configured")
        compiler_label.tooltip_text = compiler_path

func _select_compiler() -> void:
    compiler_dialog.popup_centered_ratio(0.75)

func _set_compiler(path: String) -> void:
    if not FileAccess.file_exists(path):
        _set_output("Compiler executable not found: " + path)
        return
    compiler_path = path
    EditorInterface.get_editor_settings().set_setting(SETTINGS_KEY, path)
    _refresh_labels()
    _set_output("Compiler configured. Use Check to validate an .nqr file.\nThe executable is not bundled or trusted automatically.")

func _check() -> void:
    _invoke_compiler("check")

func _confirm_run() -> void:
    run_confirmation.popup_centered()

func _run_legacy() -> void:
    _invoke_compiler("run")

func _invoke_compiler(command: String) -> void:
    if compiler_path.is_empty():
        _set_output("Select a trusted separately installed Noqeri compiler first.")
        return
    if current_path.is_empty() or dirty:
        _set_output("Save the .nqr source before running the compiler.")
        return
    if not FileAccess.file_exists(current_path):
        _set_output("Source file does not exist: " + current_path)
        return
    var arguments := PackedStringArray([command, ProjectSettings.globalize_path(current_path)])
    var messages: Array = []
    _set_output("Noqeri " + command + " " + current_path + "\n")
    # OS.execute is a direct executable+argv call, not a shell invocation.
    # It is synchronous. Use short check commands, not long-running jobs.
    var status := OS.execute(compiler_path, arguments, messages, true)
    var body := ""
    for message in messages:
        body += str(message)
    _set_output("Noqeri " + command + " exit code: " + str(status) + "\n" + body)
    if status != 0 and command == "run":
        _set_output("The selected Noqeri compiler may not support 'run'.\n" + body)

func _set_output(message: String) -> void:
    if output != null:
        output.text = message

func _handles(object: Object) -> bool:
    return object is NoeNoqeriSource

func _edit(object: Object) -> void:
    if object is NoeNoqeriSource and not dirty:
        _load_source(object.source_path)
