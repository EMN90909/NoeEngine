// Copyright (c) 2026 Noethric.
// SPDX-License-Identifier: LicenseRef-Noethric-Proprietary
#include "noe_jamila_panel.h"

#include "core/config/project_settings.h"
#include "core/crypto/crypto.h"
#include "core/io/file_access.h"
#include "core/io/json.h"
#include "core/os/os.h"
#include "editor/editor_scale.h"
#include "editor/noe_hardware_profile.h"
#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/label.h"
#include "scene/gui/line_edit.h"
#include "scene/gui/scroll_container.h"
#include "scene/gui/text_edit.h"
#include "scene/main/http_request.h"
#include "scene/main/timer.h"

NoeJamilaPanel::NoeJamilaPanel() {
    set_name("JamilaAgentOverlay");
    const bool modern = !NoeHardwareProfile::use_classic_ui();
    // Right and bottom anchored: reaches past Inspector and filter panels.
    set_anchor(SIDE_LEFT, Control::ANCHOR_END);
    set_anchor(SIDE_RIGHT, Control::ANCHOR_END);
    set_anchor(SIDE_TOP, Control::ANCHOR_BEGIN);
    set_anchor(SIDE_BOTTOM, Control::ANCHOR_END);
    const int width = (modern ? 402 : 312) * EDSCALE;
    set_offset(SIDE_LEFT, -width);
    set_offset(SIDE_RIGHT, 0);
    set_offset(SIDE_TOP, 53 * EDSCALE);
    set_offset(SIDE_BOTTOM, 0);
    set_mouse_filter(Control::MOUSE_FILTER_STOP);
    set_h_size_flags(Control::SIZE_SHRINK_END);
    set_visible(false);

    VBoxContainer *layout = memnew(VBoxContainer);
    add_child(layout);
    layout->add_theme_constant_override("separation", 8 * EDSCALE);

    HBoxContainer *title = memnew(HBoxContainer);
    layout->add_child(title);
    Label *heading = memnew(Label);
    heading->set_text("JAMILA / GAME AGENT");
    heading->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    title->add_child(heading);
    Button *close = memnew(Button);
    close->set_text("Hide");
    close->set_tooltip_text("Close Jamila and show the Inspector again");
    close->connect("pressed", callable_mp(this, &NoeJamilaPanel::_toggle));
    title->add_child(close);

    ScrollContainer *scroll = memnew(ScrollContainer);
    scroll->set_v_size_flags(Control::SIZE_EXPAND_FILL);
    layout->add_child(scroll);
    VBoxContainer *body = memnew(VBoxContainer);
    body->add_theme_constant_override("separation", 9 * EDSCALE);
    body->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    scroll->add_child(body);

    Label *guide = memnew(Label);
    guide->set_text("PLAN  /  REVIEW  /  BUILD  /  VERIFY\nChanges run through the NoeEngine project MCP after approval.");
    guide->set_autowrap_mode(TextServer::AUTOWRAP_WORD_SMART);
    body->add_child(guide);

    Label *api_label = memnew(Label);
    api_label->set_text("Model API (HTTPS, OpenAI compatible)");
    body->add_child(api_label);
    endpoint_field = memnew(LineEdit);
    endpoint_field->set_text("https://api.openai.com/v1/chat/completions");
    body->add_child(endpoint_field);
    model_field = memnew(LineEdit);
    model_field->set_placeholder("Model identifier");
    body->add_child(model_field);
    key_field = memnew(LineEdit);
    key_field->set_placeholder("API key (memory only)");
    key_field->set_secret(true);
    body->add_child(key_field);

    Label *task_label = memnew(Label);
    task_label->set_text("Describe the game or change");
    body->add_child(task_label);
    task_field = memnew(TextEdit);
    task_field->set_custom_minimum_size(Size2(0, 128 * EDSCALE));
    task_field->set_placeholder("Make a simple playable 2D platformer scene, script and controls...");
    body->add_child(task_field);

    plan_button = memnew(Button);
    plan_button->set_text("Generate editable plan");
    plan_button->set_tooltip_text("Generate a proposal only. No files are changed.");
    plan_button->connect("pressed", callable_mp(this, &NoeJamilaPanel::_plan));
    body->add_child(plan_button);

    Label *review_label = memnew(Label);
    review_label->set_text("Proposed files and changes");
    body->add_child(review_label);
    review_field = memnew(TextEdit);
    review_field->set_editable(false);
    review_field->set_custom_minimum_size(Size2(0, 205 * EDSCALE));
    body->add_child(review_field);

    apply_button = memnew(Button);
    apply_button->set_text("Approve changes, build and validate");
    apply_button->set_tooltip_text("Creates approved game assets, then runs NoeEngine --headless --editor --quit.");
    apply_button->set_disabled(true);
    apply_button->connect("pressed", callable_mp(this, &NoeJamilaPanel::_apply));
    body->add_child(apply_button);
    Label *preset_label = memnew(Label);
    preset_label->set_text("Game export preset");
    body->add_child(preset_label);
    export_preset = memnew(LineEdit);
    export_preset->set_placeholder("Exact preset name from Project > Export");
    body->add_child(export_preset);
    export_button = memnew(Button);
    export_button->set_text("Export validated game (.pck)");
    export_button->set_tooltip_text("Builds a PCK using NoeEngine --export-pack with a configured export preset.");
    export_button->set_disabled(true);
    export_button->connect("pressed", callable_mp(this, &NoeJamilaPanel::_export_game));
    body->add_child(export_button);
    release_button = memnew(Button);
    release_button->set_text("Build playable release");
    release_button->set_tooltip_text("Uses NoeEngine --export-release and installed export templates for the selected platform.");
    release_button->set_disabled(true);
    release_button->connect("pressed", callable_mp(this, &NoeJamilaPanel::_export_release));
    body->add_child(release_button);

    status_label = memnew(Label);
    status_label->set_autowrap_mode(TextServer::AUTOWRAP_WORD_SMART);
    status_label->set_text("Jamila is idle. Requires Python 3 and an AI provider key.\nGame context is sent to your selected provider.");
    body->add_child(status_label);

    http = memnew(HTTPRequest);
    http->set_timeout(310.0);
    http->set_body_size_limit(950000);
    http->set_max_redirects(0);
    http->connect("request_completed", callable_mp(this, &NoeJamilaPanel::_on_response));
    add_child(http);
    startup = memnew(Timer);
    startup->set_one_shot(true);
    startup->set_wait_time(1.2);
    startup->connect("timeout", callable_mp(this, &NoeJamilaPanel::_start_pending));
    add_child(startup);
}

NoeJamilaPanel::~NoeJamilaPanel() {
    // The local agent never survives its owning editor session.
    if (helper_pid) {
        OS::get_singleton()->kill(helper_pid);
    }
    server_token.clear();
    if (key_field) {
        key_field->clear();
    }
}

NoeJamilaPanel *NoeJamilaPanel::install(Control *root, HBoxContainer *toolbar) {
    NoeJamilaPanel *panel = memnew(NoeJamilaPanel);
    root->add_child(panel);
    if (root->has_theme_stylebox(SNAME("NoeBentoPanel"), SNAME("EditorStyles"))) {
        panel->add_theme_style_override("panel", root->get_theme_stylebox(SNAME("NoeBentoPanel"), SNAME("EditorStyles")));
    }
    Button *trigger = memnew(Button);
    trigger->set_text("Jamila");
    trigger->set_tooltip_text("Show/hide the Jamila game-making agent");
    trigger->set_toggle_mode(true);
    trigger->set_flat(true);
    trigger->connect("pressed", callable_mp(panel, &NoeJamilaPanel::_toggle));
    toolbar->add_child(trigger);
    panel->toggle_button = trigger;
    return panel;
}

void NoeJamilaPanel::_toggle() {
    const bool show = !is_visible();
    set_visible(show);
    toggle_button->set_pressed_no_signal(show);
    if (show) {
        move_to_front(); // Overlay docks; their state and sizes remain unchanged.
    }
}
String NoeJamilaPanel::_find_agent_script() const {
    const String bin = OS::get_singleton()->get_executable_path().get_base_dir();
    const String paths[] = {
        bin.path_join("Jamila/jamila_agent.py"),
        bin.path_join("../tools/jamila/jamila_agent.py"),
        bin.path_join("../Resources/Jamila/jamila_agent.py"),
        bin.path_join("../share/noeengine/Jamila/jamila_agent.py")
    };
    for (const String &path : paths) {
        if (FileAccess::exists(path)) {
            return path.simplify_path();
        }
    }
    return "";
}
bool NoeJamilaPanel::_launch_agent() {
    if (helper_pid && !server_url.is_empty()) {
        return true;
    }
    const String file = _find_agent_script();
    if (file.is_empty()) {
        status_label->set_text("Jamila runtime missing. Install a build that includes Jamila, or run from the NoeEngine source tree.");
        return false;
    }
    const String python = OS::get_singleton()->has_environment("NOE_JAMILA_PYTHON") ?
            OS::get_singleton()->get_environment("NOE_JAMILA_PYTHON") :
#ifdef WINDOWS_ENABLED
            "python";
#else
            "python3";
#endif
    // Crypto is an abstract provider; obtain the platform backend factory.
    Ref<Crypto> crypto = Crypto::create();
    if (crypto.is_null()) {
        status_label->set_text("Secure random generator unavailable.");
        return false;
    }
    const PackedByteArray secret = crypto->generate_random_bytes(32);
    const PackedByteArray port_seed = crypto->generate_random_bytes(2);
    if (secret.size() != 32 || port_seed.size() != 2) {
        status_label->set_text("Failed to initialize secure local agent.");
        return false;
    }
    server_token = String::hex_encode_buffer(secret.ptr(), secret.size());
    const int port = 24000 + ((int)port_seed[0] * 256 + (int)port_seed[1]) % 16000;
    server_url = "http://127.0.0.1:" + itos(port);
    List<String> args;
    args.push_back("-u");
    args.push_back(file);
    args.push_back("--project");
    args.push_back(ProjectSettings::get_singleton()->globalize_path("res://"));
    args.push_back("--engine");
    args.push_back(OS::get_singleton()->get_executable_path());
    args.push_back("--port");
    args.push_back(itos(port));
    args.push_back("--token");
    args.push_back(server_token);
    if (OS::get_singleton()->create_process(python, args, &helper_pid) != OK) {
        helper_pid = 0;
        server_token.clear();
        server_url.clear();
        status_label->set_text("Could not start Python 3. Set NOE_JAMILA_PYTHON to the Python executable.");
        return false;
    }
    return true;
}
void NoeJamilaPanel::_send(const String &path, const Dictionary &payload) {
    PackedStringArray headers;
    headers.push_back("Content-Type: application/json");
    headers.push_back("Authorization: Bearer " + server_token);
    const Error err = http->request(server_url + path, headers, HTTPClient::METHOD_POST, JSON::stringify(payload));
    if (err != OK) {
        running = false;
        plan_button->set_disabled(false);
        status_label->set_text("Cannot contact local Jamila. Check Python 3 runtime and project.godot.");
    }
}
void NoeJamilaPanel::_plan() {
    if (running || task_field->get_text().strip_edges().is_empty()) {
        return;
    }
    plan_id.clear();
    apply_button->set_disabled(true);
    export_button->set_disabled(true);
    release_button->set_disabled(true);
    review_field->clear();
    if (!_launch_agent()) {
        return;
    }
    running = true;
    applying = false;
    exporting = false;
    plan_button->set_disabled(true);
    status_label->set_text("Inspecting project MCP tools, planning game changes...");
    pending_plan = true;
    startup->start(); // Launch is asynchronous; never block the editor UI.
}
void NoeJamilaPanel::_start_pending() {
    if (!pending_plan) {
        return;
    }
    pending_plan = false;
    Dictionary payload;
    payload["prompt"] = task_field->get_text().strip_edges().left(3000);
    payload["api_url"] = endpoint_field->get_text().strip_edges();
    payload["model"] = model_field->get_text().strip_edges();
    payload["api_key"] = key_field->get_text();
    _send("/v1/plan", payload);
}
void NoeJamilaPanel::_apply() {
    if (running || plan_id.is_empty()) {
        return;
    }
    running = true;
    applying = true;
    exporting = false;
    apply_button->set_disabled(true);
    plan_button->set_disabled(true);
    status_label->set_text("Applying approved MCP writes, then running headless project validation...");
    Dictionary payload;
    payload["plan_id"] = plan_id;
    plan_id.clear(); // Prevent double-spend / double click of a plan.
    _send("/v1/apply", payload);
}
void NoeJamilaPanel::_export_game() {
    if (running || export_preset->get_text().strip_edges().is_empty()) return;
    running = true;
    applying = false;
    exporting = true;
    export_button->set_disabled(true);
    release_button->set_disabled(true);
    exporting_release = false;
    plan_button->set_disabled(true);
    status_label->set_text("Validating and exporting game data package...");
    Dictionary args;
    args["preset"] = export_preset->get_text().strip_edges();
    args["mode"] = "pack";
    _send("/v1/export", args);
}
void NoeJamilaPanel::_export_release() {
    if (running || export_preset->get_text().strip_edges().is_empty()) {
        return;
    }
    running = true;
    applying = false;
    exporting = true;
    exporting_release = true;
    export_button->set_disabled(true);
    release_button->set_disabled(true);
    plan_button->set_disabled(true);
    status_label->set_text("Validating project and building a playable release...");
    Dictionary args;
    args["preset"] = export_preset->get_text().strip_edges();
    args["mode"] = "release";
    _send("/v1/export", args);
}

void NoeJamilaPanel::_on_response(int result, int status, const PackedStringArray &headers, const PackedByteArray &body) {
    running = false;
    plan_button->set_disabled(false);
    const Variant parsed = JSON::parse_string(String::utf8((const char *)body.ptr(), body.size()));
    if (result != HTTPRequest::RESULT_SUCCESS || parsed.get_type() != Variant::DICTIONARY) {
        status_label->set_text("Jamila backend returned an invalid response. No build result can be confirmed.");
        return;
    }
    const Dictionary response = parsed;
    if (status != 200) {
        status_label->set_text("Jamila: " + String(response.get("error", "Unexpected backend failure.")));
        return;
    }
    if (exporting) {
        const Dictionary info = response.get("export", Dictionary());
        if ((bool)response.get("ok", false)) {
            const String built_path = info.get("path", "");
            status_label->set_text(exporting_release ? "Playable release exported." : "Game data package exported.");
            review_field->set_text("NoeEngine export: " + built_path + "\n" + String(info.get("output", "")).right(1500));
            export_button->set_disabled(false);
            release_button->set_disabled(false);
        } else {
            status_label->set_text("Game export failed. Check your preset, installed templates and build output.");
            review_field->set_text(String(info.get("output", response.get("error", "Export unavailable"))));
        }
        exporting = false;
        return;
    }
    if (!applying) {
        plan_id = response.get("plan_id", "");
        const Array actions = response.get("actions", Array());
        String review = String(response.get("summary", "")) + "\n\n";
        for (int i = 0; i < actions.size(); ++i) {
            Dictionary action = actions[i];
            Dictionary params = action.get("arguments", Dictionary());
            review += "FILE: " + String(params.get("path", "?"));
            review += (bool)params.get("overwrite", false) ? " (OVERWRITE)\n" : " (CREATE)\n";
            review += String(params.get("content", "")) + "\n\n";
        }
        review_field->set_text(review);
        apply_button->set_disabled(plan_id.is_empty() || actions.is_empty());
        status_label->set_text("Plan ready. Review all proposed content above. Nothing has been changed.");
    } else {
        const Dictionary validation = response.get("validation", Dictionary());
        String report = (bool)response.get("ok", false) ? "MCP changes applied and headless validation passed." :
                  "MCP actions completed with errors or project validation failed.";
        const Array files = response.get("files", Array());
        for (int i = 0; i < files.size(); ++i) {
            const Dictionary item = files[i];
            report += "\n" + String(item.get("path", "?")) +
                      ((bool)item.get("ok", false) ? ": saved" : ": FAILED " + String(item.get("error", "")));
        }
        report += "\n" + String(validation.get("output", validation.get("error", ""))).right(3500);
        review_field->set_text(report);
        const Dictionary playtest = response.get("playtest", Dictionary());
        if (playtest.has("output")) {
            report += "\nSCENE PLAYTEST:\n" + String(playtest.get("output", "")).right(2500);
        } else if (playtest.has("error")) {
            report += "\nSCENE PLAYTEST ERROR: " + String(playtest.get("error", ""));
        }
        export_button->set_disabled(!(bool)response.get("ok", false));
        release_button->set_disabled(!(bool)response.get("ok", false));
        status_label->set_text((bool)response.get("ok", false) ? "Game changes saved and validated. Export a .pck when ready." :
                                "Jamila could not validate the game. Review the reported errors.");
    }
}
