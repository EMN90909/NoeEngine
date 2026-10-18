// Copyright (c) 2026 Noethric.
// SPDX-License-Identifier: LicenseRef-Noethric-Proprietary
#include "noe_account_workspace.h"
#include "editor/editor_node.h"
#include "editor/editor_paths.h"
#include "editor/editor_scale.h"
#include "core/io/http_client.h"
#include "core/io/json.h"
#include "core/io/resource.h"
#include "core/os/os.h"
#include "core/os/time.h"
#include "core/string/translation.h"
#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/check_box.h"
#include "scene/gui/label.h"
#include "scene/gui/line_edit.h"
#include "scene/gui/menu_button.h"
#include "scene/gui/panel_container.h"
#include "scene/gui/popup_menu.h"
#include "scene/gui/scroll_container.h"
#include "scene/gui/separator.h"
#include "scene/gui/tab_container.h"
#include "scene/main/http_request.h"
#include "scene/main/timer.h"

class NoeAccountButton : public MenuButton {
    GDCLASS(NoeAccountButton, MenuButton);
protected:
    static void _bind_methods() {}
    void _notification(int what) {
        if (what == NOTIFICATION_ENTER_TREE || what == NOTIFICATION_THEME_CHANGED) {
            set_icon(get_theme_icon(SNAME("Godot"), SNAME("EditorIcons")));
        }
    }
};

class NoeUsageCharts : public Control {
    GDCLASS(NoeUsageCharts, Control);
    double total_ms = 0;
    double play_ms = 0;
protected:
    static void _bind_methods() {}
    void _notification(int what) {
        if (what != NOTIFICATION_DRAW) return;
        const Size2 sz = get_size();
        const Color accent = get_theme_color(SNAME("accent_color"), SNAME("Editor"));
        const Color muted = accent.darkened(0.60f);
        const Vector2 center(sz.x * 0.25f, sz.y * 0.48f);
        const float radius = 60 * EDSCALE;
        draw_arc(center, radius, 0, Math_TAU, 64, muted, 14 * EDSCALE, true);
        if (total_ms > 0 && play_ms > 0) {
            draw_arc(center, radius, -Math_PI / 2, -Math_PI / 2 + Math_TAU * CLAMP(play_ms / total_ms, 0.0, 1.0),
                    64, accent, 14 * EDSCALE, true);
        }
        const float h = sz.y * 0.56f, y = sz.y * 0.84f, w = 35 * EDSCALE, x = sz.x * 0.62f;
        draw_rect(Rect2(x, y - h, w, h), muted);
        draw_rect(Rect2(x + 2 * w, y - h, w, h), muted);
        if (total_ms > 0) {
            draw_rect(Rect2(x, y - h, w, h), accent);
            float ph = h * (float)CLAMP(play_ms / total_ms, 0.0, 1.0);
            draw_rect(Rect2(x + 2 * w, y - ph, w, ph), accent.darkened(0.25f));
        }
    }
public:
    NoeUsageCharts() { set_custom_minimum_size(Size2(470, 205) * EDSCALE); }
    void update_data(uint64_t total, uint64_t play) { total_ms = total; play_ms = play; queue_redraw(); }
};

Label *NoeAccountWorkspace::_add_label(VBoxContainer *root, const String &txt) {
    Label *label = memnew(Label);
    label->set_text(txt);
    label->set_autowrap_mode(TextServer::AUTOWRAP_WORD_SMART);
    root->add_child(label);
    return label;
}
Button *NoeAccountWorkspace::_add_button(VBoxContainer *root, const String &txt, const Callable &fn) {
    Button *b = memnew(Button);
    b->set_text(txt);
    b->set_custom_minimum_size(Size2(170, 34) * EDSCALE);
    b->set_focus_mode(Control::FOCUS_ALL);
    b->connect("pressed", fn);
    root->add_child(b);
    return b;
}
bool NoeAccountWorkspace::_is_https_url(const String &url) const {
    return url.begins_with("https://") && url.length() > 10 &&
        !url.contains(" ") && !url.contains("\n") && !url.contains("\r") && !url.contains("@");
}
void NoeAccountWorkspace::_make_page(Page page) {
    VBoxContainer *p = pages[page];
    p->add_theme_constant_override("separation", 10 * EDSCALE);
    if (page == PROFILE) {
        _add_label(p, TTR("Profile"));
        PanelContainer *avatar_frame = memnew(PanelContainer);
        avatar_frame->set_custom_minimum_size(Size2(64, 64) * EDSCALE);
        p->add_child(avatar_frame);
        profile_avatar_label = memnew(Label);
        profile_avatar_label->set_text("N");
        profile_avatar_label->set_horizontal_alignment(HORIZONTAL_ALIGNMENT_CENTER);
        profile_avatar_label->set_vertical_alignment(VERTICAL_ALIGNMENT_CENTER);
        profile_avatar_label->set_custom_minimum_size(Size2(64, 64) * EDSCALE);
        avatar_frame->add_child(profile_avatar_label);
        name_label = _add_label(p, TTR("Name: Not signed in"));
        email_label = _add_label(p, TTR("Email: Not signed in"));
        account_status = _add_label(p, TTR("Offline. No verified session."));
        _add_button(p, TTR("Sign in / Create account"), callable_mp(this, &NoeAccountWorkspace::_open_sign_in));
    } else if (page == USAGE) {
        _add_label(p, TTR("Usage & limits"));
        _add_label(p, TTR("Local time breakdown: donut chart and bar graph"));
        charts = memnew(NoeUsageCharts);
        p->add_child(charts);
        _add_label(p, TTR("Donut: playtest vs app time. Bars: app time and playtest time."));
        _add_label(p, TTR("AI provider usage (local vs cloud): pie and bar chart"));
        ai_charts = memnew(NoeUsageCharts);
        p->add_child(ai_charts);
        ai_label = _add_label(p, TTR("AI calls: no telemetry connected. Provider quota unavailable."));
        session_label = _add_label(p, TTR("Current session: 0 min"));
        totals_label = _add_label(p, TTR("Total app time: 0 min"));
        render_label = _add_label(p, TTR("Playtest runtime: 0 min. GPU rendering hours not instrumented."));
    } else if (page == API_MCP) {
        _add_label(p, TTR("API & MCP"));
        TabContainer *tabs = memnew(TabContainer);
        tabs->set_custom_minimum_size(Size2(510, 295) * EDSCALE);
        p->add_child(tabs);
        VBoxContainer *api = memnew(VBoxContainer);
        api->set_name(TTR("API")); tabs->add_child(api);
        _add_label(api, TTR("Custom endpoints (configuration only; no requests or secrets stored)."));
        _add_button(api, TTR("Add API"), callable_mp(this, &NoeAccountWorkspace::_show_api_form));
        api_list = memnew(VBoxContainer); api->add_child(api_list);
        VBoxContainer *mcp = memnew(VBoxContainer);
        mcp->set_name(TTR("MCP")); tabs->add_child(mcp);
        _add_label(mcp, TTR("My MCPs"));
        _add_button(mcp, TTR("Add custom MCP"), callable_mp(this, &NoeAccountWorkspace::_show_mcp_form));
        _add_button(mcp, TTR("Browse MCP Marketplace"), callable_mp(this, &NoeAccountWorkspace::_show_mcp_marketplace));
        _add_button(mcp, TTR("Official MCP Registry"), callable_mp(this, &NoeAccountWorkspace::_open_registry));
        mcp_list = memnew(VBoxContainer); mcp->add_child(mcp_list);
        _add_label(mcp, TTR("Enable toggles only save configuration. No MCP process is executed or connected yet."));
    } else if (page == EXTENSIONS) {
        _add_label(p, TTR("Extensions"));
        _add_label(p, TTR("Godot-compatible editor extensions install per project via Project Settings > Plugins."));
        _add_button(p, TTR("Discover Godot MCP Toolkit"), callable_mp(this, &NoeAccountWorkspace::_open_godot_mcp));
        _add_label(p, TTR("Extension installation and permissions are not managed by this panel yet."));
        _add_label(p, TTR("Team collaboration"));
        _add_label(p, TTR("Enter the Convex project ID shared by your team. Scene/resource saves publish activity, not file contents."));
        collab_project = memnew(LineEdit);
        collab_project->set_placeholder(TTR("Team project ID"));
        collab_project->set_max_length(128);
        p->add_child(collab_project);
        _add_button(p, TTR("Save collaboration project"), callable_mp(this, &NoeAccountWorkspace::_save_collaboration_project));
        collab_status = _add_label(p, TTR("No collaboration project configured."));
        collab_activity = _add_label(p, TTR("Team activity will appear here."));
    } else if (page == SECURITY) {
        _add_label(p, TTR("Security"));
        security_label = _add_label(p, TTR("Offline. No active access token."));
        _add_label(p, TTR("HTTPS device authorization. Tokens are only held in memory, not in settings."));
        _add_label(p, TTR("Account website URL (HTTPS)"));
        website_url_field = memnew(LineEdit);
        website_url_field->set_placeholder("https://your-noeengine-site.onrender.com");
        website_url_field->set_max_length(2048);
        website_url_field->set_tooltip_text(TTR("The origin hosting your NoeEngine website and device authentication API."));
        p->add_child(website_url_field);
        website_url_status = _add_label(p, TTR("Enter the deployed account website address to enable browser sign-in."));
        _add_button(p, TTR("Save account website"), callable_mp(this, &NoeAccountWorkspace::_save_website_url));
        _add_button(p, TTR("Log out"), callable_mp(this, &NoeAccountWorkspace::_logout));
    }
}
NoeAccountWorkspace::NoeAccountWorkspace() {
    set_title(TTR("NoeEngine Account & Settings"));
    set_hide_on_ok(true);
    get_ok_button()->set_text(TTR("Close"));
    config.instantiate();
    start_msec = Time::get_singleton()->get_ticks_msec();
    last_tick_msec = start_msec;
    next_save_msec = start_msec + 60000;
    if (EditorPaths::get_singleton() && EditorPaths::get_singleton()->are_paths_valid()) {
        config_path = EditorPaths::get_singleton()->get_config_dir().path_join("noeengine_account.cfg");
        config->load(config_path);
        total_session_ms = (uint64_t)(int64_t)config->get_value("usage", "app_ms", (int64_t)0);
        total_render_ms = (uint64_t)(int64_t)config->get_value("usage", "playtest_ms", (int64_t)0);
    }
    auth_base = OS::get_singleton()->get_environment("NOEENGINE_AUTH_URL").strip_edges();
    if (auth_base.is_empty()) auth_base = String(config->get_value("account", "website_url", "")).strip_edges();
    if (auth_base.is_empty()) auth_base = "https://noeengine-web.onrender.com";
    if (!_is_https_url(auth_base)) auth_base = "";
    if (auth_base.ends_with("/")) auth_base = auth_base.substr(0, auth_base.length() - 1);
    HBoxContainer *layout = memnew(HBoxContainer);
    layout->set_custom_minimum_size(Size2(660, 390) * EDSCALE);
    layout->add_theme_constant_override("separation", 16 * EDSCALE);
    add_child(layout);
    VBoxContainer *nav = memnew(VBoxContainer);
    nav->set_custom_minimum_size(Size2(170, 0) * EDSCALE);
    layout->add_child(nav);
    _add_label(nav, TTR("ACCOUNT"));
    const String items[] = { TTR("Profile"), TTR("Usage & limits"), TTR("API & MCP"), TTR("Extensions"), TTR("Security") };
    for (int i = 0; i < 5; ++i) {
        navigation_buttons[i] = _add_button(nav, items[i], callable_mp(this, &NoeAccountWorkspace::_show_page).bind(i));
        navigation_buttons[i]->set_toggle_mode(true);
        navigation_buttons[i]->set_text_alignment(HORIZONTAL_ALIGNMENT_LEFT);
    }
    layout->add_child(memnew(VSeparator));
    ScrollContainer *scroll = memnew(ScrollContainer);
    scroll->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    layout->add_child(scroll);
    VBoxContainer *body = memnew(VBoxContainer);
    body->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    scroll->add_child(body);
    for (int i = 0; i < 5; ++i) {
        pages[i] = memnew(VBoxContainer);
        body->add_child(pages[i]);
        _make_page((Page)i);
        pages[i]->hide();
    }
    pages[PROFILE]->show();
    navigation_buttons[PROFILE]->set_pressed_no_signal(true);
    // Security page controls are constructed before initial URL configuration is shown.
    if (website_url_field) website_url_field->set_text(auth_base);
    collaboration_url = OS::get_singleton()->get_environment("NOEENGINE_COLLAB_URL").strip_edges();
    if (!_is_https_url(collaboration_url)) collaboration_url = "";
    if (collaboration_url.ends_with("/")) collaboration_url = collaboration_url.substr(0, collaboration_url.length() - 1);
    if (collab_project) collab_project->set_text(String(config->get_value("collaboration", "project_id", "")));
    welcome = memnew(ConfirmationDialog);
    welcome->set_title(TTR("Welcome to NoeEngine"));
    welcome->set_text(TTR("Sign in to connect your account, or use NoeEngine offline."));
    welcome->set_ok_button_text(TTR("Sign in"));
    welcome->set_cancel_button_text(TTR("Continue offline"));
    welcome->connect("confirmed", callable_mp(this, &NoeAccountWorkspace::_open_sign_in));
    add_child(welcome);
    login_dialog = memnew(AcceptDialog);
    login_dialog->set_title(TTR("Connect NoeEngine account"));
    login_message = memnew(Label);
    login_message->set_custom_minimum_size(Size2(410, 110) * EDSCALE);
    login_message->set_autowrap_mode(TextServer::AUTOWRAP_WORD_SMART);
    login_dialog->add_child(login_message);
    add_child(login_dialog);
    api_dialog = memnew(ConfirmationDialog);
    api_dialog->set_title(TTR("Add API"));
    VBoxContainer *af = memnew(VBoxContainer);
    af->set_custom_minimum_size(Size2(440, 165) * EDSCALE);
    api_dialog->add_child(af);
    _add_label(af, TTR("Name"));
    api_name = memnew(LineEdit); api_name->set_max_length(64); api_name->set_tooltip_text(TTR("API name")); af->add_child(api_name);
    _add_label(af, TTR("HTTPS endpoint"));
    api_url = memnew(LineEdit); api_url->set_max_length(2048); api_url->set_tooltip_text(TTR("HTTPS API endpoint")); api_url->set_placeholder("https://api.example.com/v1"); af->add_child(api_url);
    api_error = _add_label(af, "");
    api_dialog->connect("confirmed", callable_mp(this, &NoeAccountWorkspace::_save_api));
    add_child(api_dialog);
    mcp_dialog = memnew(ConfirmationDialog);
    mcp_dialog->set_title(TTR("Add remote MCP"));
    VBoxContainer *mf = memnew(VBoxContainer);
    mf->set_custom_minimum_size(Size2(440, 165) * EDSCALE);
    mcp_dialog->add_child(mf);
    _add_label(mf, TTR("Name"));
    mcp_name = memnew(LineEdit); mcp_name->set_max_length(64); mcp_name->set_tooltip_text(TTR("MCP name")); mf->add_child(mcp_name);
    _add_label(mf, TTR("HTTPS MCP endpoint"));
    mcp_url = memnew(LineEdit); mcp_url->set_max_length(2048); mcp_url->set_tooltip_text(TTR("HTTPS MCP endpoint")); mcp_url->set_placeholder("https://mcp.example.com/mcp"); mf->add_child(mcp_url);
    mcp_error = _add_label(mf, "");
    mcp_dialog->connect("confirmed", callable_mp(this, &NoeAccountWorkspace::_save_mcp));
    add_child(mcp_dialog);
    marketplace_dialog = memnew(AcceptDialog);
    marketplace_dialog->set_title(TTR("Godot MCP Marketplace"));
    VBoxContainer *marketplace_root = memnew(VBoxContainer);
    marketplace_root->set_custom_minimum_size(Size2(560, 370) * EDSCALE);
    marketplace_root->add_theme_constant_override("separation", 10 * EDSCALE);
    marketplace_dialog->add_child(marketplace_root);
    _add_label(marketplace_root, TTR("Discover Godot-related MCP servers from the official MCP Registry."));
    ScrollContainer *marketplace_scroll = memnew(ScrollContainer);
    marketplace_scroll->set_custom_minimum_size(Size2(535, 250) * EDSCALE);
    marketplace_root->add_child(marketplace_scroll);
    marketplace_results = memnew(VBoxContainer);
    marketplace_results->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    marketplace_scroll->add_child(marketplace_results);
    _add_button(marketplace_root, TTR("Open Godot MCP Toolkit page"), callable_mp(this, &NoeAccountWorkspace::_open_godot_mcp));
    add_child(marketplace_dialog);
    registry_request = memnew(HTTPRequest);
    registry_request->set_timeout(15.0);
    registry_request->set_body_size_limit(1024 * 1024);
    registry_request->set_max_redirects(0);
    registry_request->connect("request_completed", callable_mp(this, &NoeAccountWorkspace::_on_registry_response));
    add_child(registry_request);
    auth_request = memnew(HTTPRequest);
    auth_request->set_timeout(15.0);
    auth_request->set_body_size_limit(65536);
    auth_request->set_max_redirects(0); // Never forward bearer tokens to redirected origins.
    auth_request->connect("request_completed", callable_mp(this, &NoeAccountWorkspace::_on_auth_response));
    add_child(auth_request);
    poll_timer = memnew(Timer);
    poll_timer->connect("timeout", callable_mp(this, &NoeAccountWorkspace::_poll_device_flow));
    add_child(poll_timer);
    collab_request = memnew(HTTPRequest);
    collab_request->set_timeout(10.0);
    collab_request->set_body_size_limit(65536);
    collab_request->set_max_redirects(0);
    collab_request->connect("request_completed", callable_mp(this, &NoeAccountWorkspace::_on_collaboration_response));
    add_child(collab_request);
    collab_timer = memnew(Timer);
    collab_timer->set_wait_time(15.0);
    collab_timer->connect("timeout", callable_mp(this, &NoeAccountWorkspace::_poll_collaboration_activity));
    add_child(collab_timer);
    metrics_timer = memnew(Timer);
    metrics_timer->set_wait_time(1);
    metrics_timer->connect("timeout", callable_mp(this, &NoeAccountWorkspace::_tick_usage));
    add_child(metrics_timer);
    _refresh_profile();
    _render_integrations();
}
NoeAccountWorkspace *NoeAccountWorkspace::install(Control *parent, HBoxContainer *toolbar) {
    NoeAccountWorkspace *w = memnew(NoeAccountWorkspace);
    parent->add_child(w);
    MenuButton *b = memnew(NoeAccountButton);
    b->set_text(TTR("Account"));
    b->set_tooltip_text(TTR("Account, usage and integration settings"));
    b->set_flat(true);
    toolbar->add_child(b);
    w->avatar_button = b;
    PopupMenu *menu = b->get_popup();
    menu->add_item(TTR("Profile settings"), PROFILE);
    menu->add_item(TTR("Usage & limits"), USAGE);
    menu->add_item(TTR("API & MCP"), API_MCP);
    menu->add_separator();
    menu->add_item(TTR("Log out"), 20);
    menu->connect("id_pressed", callable_mp(w, &NoeAccountWorkspace::_account_action));
    return w;
}
void NoeAccountWorkspace::_notification(int what) {
    if (what == NOTIFICATION_READY) {
        metrics_timer->start();
        collab_timer->start();
        if (EditorNode::get_singleton()) {
            EditorNode::get_singleton()->connect("scene_saved", callable_mp(this, &NoeAccountWorkspace::_on_scene_saved));
            EditorNode::get_singleton()->connect("resource_saved", callable_mp(this, &NoeAccountWorkspace::_on_resource_saved));
        }
        Timer *intro = memnew(Timer);
        add_child(intro);
        intro->set_one_shot(true);
        intro->set_wait_time(0.8);
        intro->connect("timeout", callable_mp(this, &NoeAccountWorkspace::_show_welcome));
        intro->start();
    }
}
NoeAccountWorkspace::~NoeAccountWorkspace() { _save_usage(); access_token.clear(); }
void NoeAccountWorkspace::_show_page(int idx) {
    if (idx < 0 || idx >= 5) return;
    for (int i = 0; i < 5; ++i) {
        pages[i]->set_visible(i == idx);
        navigation_buttons[i]->set_pressed_no_signal(i == idx);
    }
    // Switching tabs should not move or resize a settings dialog that is already open.
    if (is_inside_tree() && !is_visible()) popup_centered_clamped(Size2i(810, 515) * EDSCALE, 0.90f);
}
void NoeAccountWorkspace::_account_action(int id) {
    if (id == 20) { _logout(); return; }
    _show_page(id);
}
void NoeAccountWorkspace::_show_welcome() {
    if ((bool)config->get_value("account", "welcome_seen", false)) return;
    config->set_value("account", "welcome_seen", true);
    _save_usage();
    welcome->popup_centered();
}
void NoeAccountWorkspace::_open_sign_in() {
    login_message->set_text(auth_base.is_empty() ?
        TTR("No account website is configured. Open Account > Profile settings > Security, enter the deployed HTTPS website URL, then save. No account was signed in.") :
        TTR("Requesting a sign-in code. Your browser will open the account website."));
    login_dialog->popup_centered(Size2i(480, 200) * EDSCALE);
    if (!auth_base.is_empty()) _start_device_flow();
}
void NoeAccountWorkspace::_start_device_flow() {
    if (request_kind != REQUEST_NONE) return;
    Dictionary payload; payload["client_name"] = "NoeEngine Desktop";
    PackedStringArray headers; headers.push_back("Content-Type: application/json");
    if (auth_request->request(auth_base + "/device/start", headers, HTTPClient::METHOD_POST, JSON::stringify(payload)) != OK) {
        login_message->set_text(TTR("Cannot reach the account service."));
        return;
    }
    request_kind = REQUEST_DEVICE_START;
}
void NoeAccountWorkspace::_poll_device_flow() {
    if (!awaiting_login || request_kind != REQUEST_NONE) return;
    if (Time::get_singleton()->get_ticks_msec() > authorized_until_msec) {
        awaiting_login = false; poll_timer->stop();
        login_message->set_text(TTR("Code expired. Start sign in again.")); return;
    }
    Dictionary payload; payload["device_code"] = device_code;
    PackedStringArray headers; headers.push_back("Content-Type: application/json");
    if (auth_request->request(auth_base + "/device/poll", headers, HTTPClient::METHOD_POST, JSON::stringify(payload)) == OK)
        request_kind = REQUEST_DEVICE_POLL;
}
void NoeAccountWorkspace::_request_profile() {
    PackedStringArray headers; headers.push_back("Authorization: Bearer " + access_token);
    if (auth_request->request(auth_base + "/me", headers) == OK) request_kind = REQUEST_ME;
    else _logout();
}
void NoeAccountWorkspace::_on_auth_response(int result, int status, const PackedStringArray &headers, const PackedByteArray &body) {
    RequestKind phase = request_kind;
    request_kind = REQUEST_NONE;
    if (phase == REQUEST_NONE) return;
    Variant parsed = JSON::parse_string(body.get_string_from_utf8());
    Dictionary reply;
    if (parsed.get_type() == Variant::DICTIONARY) reply = parsed;
    if (result != HTTPRequest::RESULT_SUCCESS) {
        awaiting_login = false; poll_timer->stop();
        login_message->set_text(TTR("Account service network error.")); return;
    }
    if (phase == REQUEST_DEVICE_START) {
        String url = reply.get("verification_uri", "");
        String code = reply.get("device_code", "");
        String user_code = reply.get("user_code", "");
        if (status != 200 || code.is_empty() || user_code.is_empty() || !_is_https_url(url) ||
                !(url == auth_base || url.begins_with(auth_base + "/"))) {
            login_message->set_text(TTR("Invalid authorization challenge from account server.")); return;
        }
        device_code = code;
        polling_interval = CLAMP((int)reply.get("interval", 5), 5, 30);
        authorized_until_msec = Time::get_singleton()->get_ticks_msec() +
                (uint64_t)CLAMP((int)reply.get("expires_in", 600), 60, 900) * 1000;
        awaiting_login = true;
        login_message->set_text(vformat(TTR("Enter code %s in the browser. Waiting for verified approval..."), user_code));
        if (OS::get_singleton()->shell_open(url) != OK)
            login_message->set_text(vformat(TTR("Open %s and enter code %s."), url, user_code));
        poll_timer->set_wait_time(polling_interval);
        poll_timer->start();
    } else if (phase == REQUEST_DEVICE_POLL) {
        if (status == 202 || String(reply.get("status", "")) == "pending") return;
        if (status == 429 || String(reply.get("status", "")) == "slow_down") {
            polling_interval = MIN(60, polling_interval + 5); poll_timer->set_wait_time(polling_interval); return;
        }
        String token = reply.get("access_token", "");
        if (status != 200 || token.is_empty()) {
            awaiting_login = false; poll_timer->stop();
            login_message->set_text(TTR("Authorization was rejected or expired.")); return;
        }
        access_token = token;
        device_code.clear();
        awaiting_login = false; poll_timer->stop();
        login_message->set_text(TTR("Approved. Verifying account identity..."));
        _request_profile();
    } else if (phase == REQUEST_ME) {
        String verified_email = reply.get("email", "");
        if (status != 200 || !verified_email.contains("@")) {
            _logout(); login_message->set_text(TTR("Identity verification failed.")); return;
        }
        email = verified_email;
        display_name = reply.get("name", "");
        if (display_name.is_empty()) display_name = email.get_slice("@", 0);
        online_session = true;
        login_dialog->hide();
        _refresh_profile();
        _show_page(PROFILE);
    }
}
void NoeAccountWorkspace::_refresh_profile() {
    name_label->set_text(TTR("Name: ") + (online_session ? display_name : TTR("Not signed in")));
    email_label->set_text(TTR("Email: ") + (online_session ? email : TTR("Not signed in")));
    account_status->set_text(online_session ? TTR("Verified session") : TTR("Offline / unverified"));
    security_label->set_text(online_session ? TTR("Signed in; token is in memory only.") : TTR("Offline; no token stored."));
    if (website_url_status) website_url_status->set_text(auth_base.is_empty() ?
            TTR("No account website configured. Set an HTTPS origin to connect.") : TTR("Account website: ") + auth_base);
    if (profile_avatar_label) {
        profile_avatar_label->set_text(online_session && !display_name.is_empty() ?
                display_name.left(1).to_upper() : String("N"));
    }
    if (avatar_button) avatar_button->set_text(online_session ? display_name.left(14) : TTR("Account"));
}
void NoeAccountWorkspace::_save_website_url() {
    String site = website_url_field->get_text().strip_edges();
    if (!_is_https_url(site)) {
        website_url_status->set_text(TTR("Enter a valid HTTPS URL without credentials."));
        return;
    }
    while (site.ends_with("/")) site = site.substr(0, site.length() - 1);
    // Changing the endpoint invalidates the current local identity and pending device flow.
    _logout();
    auth_base = site;
    config->set_value("account", "website_url", site);
    _save_usage();
    website_url_status->set_text(TTR("Website saved. Choose Sign in to authorize this editor."));
}
void NoeAccountWorkspace::_logout() {
    // Cancel outstanding approval/profile responses so logout cannot be undone by a late callback.
    if (request_kind != REQUEST_NONE) auth_request->cancel_request();
    request_kind = REQUEST_NONE;
    if (collab_request) collab_request->cancel_request();
    queued_collaboration_paths.clear(); queued_collaboration_ids.clear();
    access_token.clear(); device_code.clear(); email.clear(); display_name.clear();
    online_session = false; awaiting_login = false; poll_timer->stop();
    if (collab_request) collab_request->cancel_request();
    collaboration_polling = false;
    _refresh_profile();
}
void NoeAccountWorkspace::_save_usage() {
    if (config_path.is_empty()) return;
    config->set_value("usage", "app_ms", (int64_t)total_session_ms);
    config->set_value("usage", "playtest_ms", (int64_t)total_render_ms);
    if (config->save(config_path) != OK) WARN_PRINT("Cannot save NoeEngine account preferences.");
}
void NoeAccountWorkspace::_tick_usage() {
    uint64_t now = Time::get_singleton()->get_ticks_msec();
    uint64_t delta = now >= last_tick_msec ? MIN(now - last_tick_msec, (uint64_t)5000) : 0;
    last_tick_msec = now;
    total_session_ms += delta;
    if (EditorNode::get_singleton() && EditorNode::get_singleton()->is_run_playing()) total_render_ms += delta;
    session_label->set_text(vformat(TTR("Current session: %d min"), (int)((now - start_msec) / 60000)));
    totals_label->set_text(vformat(TTR("Total app time: %d min"), (int)(total_session_ms / 60000)));
    render_label->set_text(vformat(TTR("Playtest runtime: %d min. GPU rendering hours not instrumented."), (int)(total_render_ms / 60000)));
    charts->update_data(total_session_ms, total_render_ms);
    // Providers will report counts here once AI requests are instrumented.
    // Until then, explicitly render zero activity instead of fabricated usage.
    const uint64_t ai_local = (uint64_t)(int64_t)config->get_value("ai_usage", "local_requests", (int64_t)0);
    const uint64_t ai_cloud = (uint64_t)(int64_t)config->get_value("ai_usage", "cloud_requests", (int64_t)0);
    ai_charts->update_data(ai_local + ai_cloud, ai_cloud);
    ai_label->set_text(vformat(TTR("AI calls: %d local, %d cloud. Provider quota unavailable until connected."),
            (int)ai_local, (int)ai_cloud));
    if (now >= next_save_msec) { _save_usage(); next_save_msec = now + 60000; }
}
void NoeAccountWorkspace::_show_api_form() {
    api_name->clear(); api_url->clear(); api_error->set_text("");
    api_dialog->popup_centered(Size2i(485, 300) * EDSCALE);
    api_name->grab_focus();
}
void NoeAccountWorkspace::_save_api() {
    String name = api_name->get_text().strip_edges(), url = api_url->get_text().strip_edges();
    if (name.is_empty() || !_is_https_url(url)) {
        api_error->set_text(TTR("Name and HTTPS endpoint required."));
        api_dialog->popup_centered(); return;
    }
    int count = CLAMP((int)config->get_value("api", "count", 0), 0, 128);
    if (count == 128) return;
    config->set_value("api", "count", count + 1);
    config->set_value("api", "name_" + itos(count), name);
    config->set_value("api", "url_" + itos(count), url);
    _save_usage(); _render_integrations();
}
void NoeAccountWorkspace::_show_mcp_form() {
    mcp_name->clear(); mcp_url->clear(); mcp_error->set_text("");
    mcp_dialog->popup_centered(Size2i(485, 300) * EDSCALE);
    mcp_name->grab_focus();
}
void NoeAccountWorkspace::_save_mcp() {
    String name = mcp_name->get_text().strip_edges(), url = mcp_url->get_text().strip_edges();
    if (name.is_empty() || !_is_https_url(url)) {
        mcp_error->set_text(TTR("Name and HTTPS MCP endpoint required."));
        mcp_dialog->popup_centered(); return;
    }
    int count = CLAMP((int)config->get_value("mcp", "count", 0), 0, 128);
    if (count == 128) return;
    config->set_value("mcp", "count", count + 1);
    config->set_value("mcp", "name_" + itos(count), name);
    config->set_value("mcp", "url_" + itos(count), url);
    config->set_value("mcp", "enabled_" + itos(count), false);
    _save_usage(); _render_integrations();
}
void NoeAccountWorkspace::_toggle_mcp(bool enabled, int index) {
    if (index < 0 || index >= (int)config->get_value("mcp", "count", 0)) return;
    config->set_value("mcp", "enabled_" + itos(index), enabled);
    _save_usage();
}
void NoeAccountWorkspace::_render_integrations() {
    if (!api_list || !mcp_list) return;
    for (int i = api_list->get_child_count() - 1; i >= 0; --i) api_list->get_child(i)->queue_free();
    for (int i = mcp_list->get_child_count() - 1; i >= 0; --i) mcp_list->get_child(i)->queue_free();
    int count = CLAMP((int)config->get_value("api", "count", 0), 0, 128);
    if (!count) _add_label(api_list, TTR("No APIs added."));
    for (int i = 0; i < count; ++i)
        _add_label(api_list, String(config->get_value("api", "name_" + itos(i), "")) + " · " +
                String(config->get_value("api", "url_" + itos(i), "")));
    count = CLAMP((int)config->get_value("mcp", "count", 0), 0, 128);
    if (!count) _add_label(mcp_list, TTR("No MCPs added."));
    for (int i = 0; i < count; ++i) {
        CheckBox *cb = memnew(CheckBox);
        cb->set_text(String(config->get_value("mcp", "name_" + itos(i), "")) + " · " +
                     String(config->get_value("mcp", "url_" + itos(i), "")));
        cb->set_pressed((bool)config->get_value("mcp", "enabled_" + itos(i), false));
        cb->set_tooltip_text(TTR("Saves a preference; does not launch the server."));
        cb->connect("toggled", callable_mp(this, &NoeAccountWorkspace::_toggle_mcp).bind(i));
        mcp_list->add_child(cb);
    }
}
void NoeAccountWorkspace::_show_mcp_marketplace() {
    for (int i = marketplace_results->get_child_count() - 1; i >= 0; --i)
        marketplace_results->get_child(i)->queue_free();
    _add_label(marketplace_results, TTR("Searching the official registry for Godot MCP servers..."));
    marketplace_dialog->popup_centered(Size2i(650, 480) * EDSCALE);
    Error status = registry_request->request(
            "https://registry.modelcontextprotocol.io/v0.1/servers?search=godot&version=latest&limit=20");
    if (status != OK)
        _add_label(marketplace_results, TTR("Registry request could not start. Open the registry in a browser instead."));
}
void NoeAccountWorkspace::_on_registry_response(int result, int code, const PackedStringArray &headers, const PackedByteArray &body) {
    for (int i = marketplace_results->get_child_count() - 1; i >= 0; --i)
        marketplace_results->get_child(i)->queue_free();
    if (result != HTTPRequest::RESULT_SUCCESS || code != 200) {
        _add_label(marketplace_results, TTR("The registry is unavailable. Use the external marketplace link."));
        _add_button(marketplace_results, TTR("Open MCP Registry"), callable_mp(this, &NoeAccountWorkspace::_open_registry));
        return;
    }
    Variant json = JSON::parse_string(body.get_string_from_utf8());
    if (json.get_type() != Variant::DICTIONARY) {
        _add_label(marketplace_results, TTR("Unexpected registry response.")); return;
    }
    Dictionary result_data = json;
    Array servers = result_data.get("servers", Array());
    int shown = 0;
    for (int i = 0; i < servers.size() && shown < 20; ++i) {
        if (servers[i].get_type() != Variant::DICTIONARY) continue;
        Dictionary item = servers[i];
        Variant descriptor = item.get("server", Dictionary());
        if (descriptor.get_type() != Variant::DICTIONARY) continue;
        Dictionary server = descriptor;
        const String name = server.get("title", server.get("name", ""));
        Variant remotes = server.get("remotes", Array());
        if (remotes.get_type() != Variant::ARRAY) continue;
        Array endpoints = remotes;
        for (int j = 0; j < endpoints.size(); ++j) {
            if (endpoints[j].get_type() != Variant::DICTIONARY) continue;
            Dictionary remote = endpoints[j];
            const String url = remote.get("url", "");
            if (!_is_https_url(url)) continue;
            _add_button(marketplace_results, vformat(TTR("Configure %s"), name.left(45)),
                    callable_mp(this, &NoeAccountWorkspace::_marketplace_select).bind(name, url));
            ++shown;
            break;
        }
    }
    if (shown == 0) {
        _add_label(marketplace_results, TTR("No directly configurable HTTPS Godot MCP servers found."));
        _add_label(marketplace_results, TTR("Some registry results are local packages, which require manual installation."));
    }
    _add_label(marketplace_results, TTR("Always inspect a server's documentation and permissions before enabling it."));
}
void NoeAccountWorkspace::_marketplace_select(const String &name, const String &url) {
    marketplace_dialog->hide();
    _show_mcp_form();
    mcp_name->set_text(name);
    mcp_url->set_text(url);
}

void NoeAccountWorkspace::_open_godot_mcp() {
    OS::get_singleton()->shell_open("https://store.godotengine.org/asset/npgamedev/godot-mcp-toolkit/");
}
void NoeAccountWorkspace::_open_registry() {
    OS::get_singleton()->shell_open("https://registry.modelcontextprotocol.io/");
}

void NoeAccountWorkspace::_save_collaboration_project() {
    if (!collab_project) return;
    const String candidate = collab_project->get_text().strip_edges();
    if (!candidate.is_empty() && (candidate.length() < 3 || candidate.length() > 128 ||
            candidate.contains("/") || candidate.contains(" ") || candidate.contains("\\") ||
            candidate.contains("?") || candidate.contains("#"))) {
        collab_status->set_text(TTR("Invalid project ID."));
        return;
    }
    queued_collaboration_paths.clear();
    queued_collaboration_ids.clear();
    config->set_value("collaboration", "project_id", candidate);
    _save_usage();
    collab_status->set_text(candidate.is_empty() ? TTR("Collaboration disabled.") :
            TTR("Project selected. Requires a verified session and HTTPS collaboration service."));
}
void NoeAccountWorkspace::_on_scene_saved(const String &path) { _send_collaboration_event(path); }
void NoeAccountWorkspace::_on_resource_saved(const Ref<Resource> &resource) {
    if (resource.is_valid()) _send_collaboration_event(resource->get_path());
}
void NoeAccountWorkspace::_send_collaboration_event(const String &file) {
    if (!online_session || access_token.is_empty() || collaboration_url.is_empty() || !collab_project) return;
    const String project = String(config->get_value("collaboration", "project_id", ""));
    if (project.is_empty() || !file.begins_with("res://")) return;
    const String relative_path = file.trim_prefix("res://");
    if (relative_path.is_empty() || relative_path.length() > 512 || relative_path.contains("..") ||
            relative_path.contains("\\") || relative_path.contains("\n") || relative_path.contains("\r")) return;
    // Preserve each save notification while the gateway is handling another request.
    // A bounded queue prevents uncontrolled memory growth while the service is offline.
    if (queued_collaboration_paths.size() >= 64) {
        collab_status->set_text(TTR("Team activity queue full. Some new save notifications were not recorded."));
        return;
    }
    queued_collaboration_paths.push_back(relative_path);
    queued_collaboration_ids.push_back(itos((int64_t)Time::get_singleton()->get_ticks_usec()) +
            "-" + itos((int64_t)++collaboration_counter));
    _flush_collaboration_queue();
}
void NoeAccountWorkspace::_flush_collaboration_queue() {
    if (!online_session || access_token.is_empty() || collaboration_url.is_empty() ||
            queued_collaboration_paths.is_empty() || collaboration_polling ||
            collab_request->get_http_client_status() != HTTPClient::STATUS_DISCONNECTED) return;
    const String project = String(config->get_value("collaboration", "project_id", ""));
    if (project.is_empty()) return;
    Dictionary payload;
    payload["projectId"] = project;
    payload["path"] = queued_collaboration_paths[0];
    payload["action"] = "saved";
    payload["requestId"] = queued_collaboration_ids[0];
    PackedStringArray headers;
    headers.push_back("Content-Type: application/json");
    headers.push_back("Authorization: Bearer " + access_token);
    if (collab_request->request(collaboration_url + "/v1/collaboration/events", headers,
            HTTPClient::METHOD_POST, JSON::stringify(payload)) != OK)
        collab_status->set_text(TTR("Team activity queued. Reconnecting on next check."));
}
void NoeAccountWorkspace::_poll_collaboration_activity() {
    if (!online_session || access_token.is_empty() || collaboration_url.is_empty() || !collab_project ||
            String(config->get_value("collaboration", "project_id", "")).is_empty()) return;
    // Saves take priority over the feed poll; retry using the same request ID.
    if (!queued_collaboration_paths.is_empty()) {
        _flush_collaboration_queue();
        return;
    }
    if (collab_request->get_http_client_status() != HTTPClient::STATUS_DISCONNECTED) return;
    PackedStringArray headers;
    headers.push_back("Authorization: Bearer " + access_token);
    collaboration_polling = true;
    const String project = String(config->get_value("collaboration", "project_id", "")).uri_encode();
    if (collab_request->request(collaboration_url + "/v1/collaboration/activity?projectId=" + project, headers) != OK)
        collaboration_polling = false;
}
void NoeAccountWorkspace::_on_collaboration_response(int result, int code, const PackedStringArray &headers,
        const PackedByteArray &body) {
    if (!online_session) return;
    const bool was_poll = collaboration_polling;
    collaboration_polling = false;
    if (result != HTTPRequest::RESULT_SUCCESS || code != 200) {
        if (!was_poll && result == HTTPRequest::RESULT_SUCCESS &&
                (code == 400 || code == 401 || code == 403 || code == 404) &&
                !queued_collaboration_paths.is_empty()) {
            // A permanently rejected event must not block later saves forever.
            // Keep transient server/network errors for stable-id retry instead.
            queued_collaboration_paths.remove_at(0);
            queued_collaboration_ids.remove_at(0);
            collab_status->set_text(TTR("Save notification rejected. Check sign-in, team permissions, and project ID."));
        } else {
            collab_status->set_text(TTR("Team service unavailable. Pending saves will retry."));
        }
        return;
    }
    if (!was_poll) {
        if (!queued_collaboration_paths.is_empty()) {
            queued_collaboration_paths.remove_at(0);
            queued_collaboration_ids.remove_at(0);
        }
        collab_status->set_text(TTR("File save shared with authorized teammates."));
        _flush_collaboration_queue();
        return;
    }
    Variant parsed = JSON::parse_string(body.get_string_from_utf8());
    if (parsed.get_type() != Variant::DICTIONARY) return;
    Dictionary json = parsed;
    Variant raw = json.get("events", Array());
    if (raw.get_type() != Variant::ARRAY) return;
    Array activity = raw;
    if (activity.is_empty()) {
        collab_activity->set_text(TTR("No teammate changes yet."));
        _flush_collaboration_queue();
        return;
    }
    String summary;
    for (int i = 0; i < MIN(activity.size(), 8); ++i) {
        if (activity[i].get_type() != Variant::DICTIONARY) continue;
        Dictionary item = activity[i];
        String actor = item.get("actorEmail", "");
        String path = item.get("path", "");
        String action = item.get("action", "");
        summary += actor.left(80) + " " + action.left(12) + " " + path.left(120) + "\n";
    }
    collab_activity->set_text(summary.strip_edges());
    _flush_collaboration_queue();
}
