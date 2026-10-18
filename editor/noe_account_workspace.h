// Copyright (c) 2026 Noethric.
// SPDX-License-Identifier: LicenseRef-Noethric-Proprietary
#ifndef NOE_ACCOUNT_WORKSPACE_H
#define NOE_ACCOUNT_WORKSPACE_H

#include "scene/gui/dialogs.h"
#include "core/io/config_file.h"

class Button;
class CheckBox;
class HTTPRequest;
class HBoxContainer;
class Label;
class LineEdit;
class MenuButton;
class NoeUsageCharts;
class PopupMenu;
class Timer;
class VBoxContainer;

// Native, theme-aware account workspace shared by the editor and project manager.
// API endpoints and authentication are disabled unless a trusted HTTPS backend
// has explicitly been configured with NOEENGINE_AUTH_URL.
class NoeAccountWorkspace : public AcceptDialog {
    GDCLASS(NoeAccountWorkspace, AcceptDialog);

    enum Page { PROFILE, USAGE, API_MCP, EXTENSIONS, SECURITY };
    enum RequestKind { REQUEST_NONE, REQUEST_DEVICE_START, REQUEST_DEVICE_POLL, REQUEST_ME };
    MenuButton *avatar_button = nullptr;
    VBoxContainer *pages[5] = {};
    Label *name_label = nullptr;
    Label *email_label = nullptr;
    Label *account_status = nullptr;
    Label *session_label = nullptr;
    Label *totals_label = nullptr;
    Label *render_label = nullptr;
    Label *ai_label = nullptr;
    VBoxContainer *mcp_list = nullptr;
    VBoxContainer *api_list = nullptr;
    NoeUsageCharts *charts = nullptr;
    NoeUsageCharts *ai_charts = nullptr;
    Label *security_label = nullptr;
    Label *login_message = nullptr;
    Label *api_error = nullptr;
    Label *mcp_error = nullptr;
    LineEdit *api_name = nullptr;
    LineEdit *api_url = nullptr;
    LineEdit *mcp_name = nullptr;
    LineEdit *mcp_url = nullptr;
    ConfirmationDialog *welcome = nullptr;
    AcceptDialog *login_dialog = nullptr;
    AcceptDialog *marketplace_dialog = nullptr;
    VBoxContainer *marketplace_results = nullptr;
    HTTPRequest *registry_request = nullptr;
    ConfirmationDialog *api_dialog = nullptr;
    ConfirmationDialog *mcp_dialog = nullptr;
    HTTPRequest *auth_request = nullptr;
    Timer *poll_timer = nullptr;
    Timer *metrics_timer = nullptr;
    Ref<ConfigFile> config;
    String config_path;
    String auth_base;
    String access_token; // Deliberately memory-only. Never saved in ConfigFile.
    String device_code;
    String email;
    String display_name;
    uint64_t start_msec = 0;
    uint64_t last_tick_msec = 0;
    uint64_t next_save_msec = 0;
    uint64_t authorized_until_msec = 0;
    uint64_t total_session_ms = 0;
    uint64_t total_render_ms = 0;
    int polling_interval = 5;
    RequestKind request_kind = REQUEST_NONE;
    bool awaiting_login = false;
    bool online_session = false;

    static void _bind_methods() {}
    void _show_page(int p_page);
    void _account_action(int p_id);
    void _show_welcome();
    void _open_sign_in();
    void _start_device_flow();
    void _poll_device_flow();
    void _request_profile();
    void _on_auth_response(int p_result, int p_code, const PackedStringArray &p_headers, const PackedByteArray &p_body);
    void _save_usage();
    void _tick_usage();
    void _logout();
    void _show_api_form();
    void _save_api();
    void _show_mcp_form();
    void _save_mcp();
    void _toggle_mcp(bool p_enabled, int p_index);
    void _render_integrations();
    void _show_mcp_marketplace();
    void _on_registry_response(int p_result, int p_code, const PackedStringArray &p_headers, const PackedByteArray &p_body);
    void _marketplace_select(const String &p_name, const String &p_url);
    void _open_godot_mcp();
    void _open_registry();
    void _refresh_profile();
    bool _is_https_url(const String &p_url) const;
    void _make_page(VBoxContainer *p_root, Page p_page);
    Label *_add_label(VBoxContainer *p_root, const String &p_text);
    Button *_add_button(VBoxContainer *p_root, const String &p_text, const Callable &p_action);

protected:
    void _notification(int p_what);

public:
    // The owning UI tree manages this dialog. The toolbar must be part of it.
    static NoeAccountWorkspace *install(Control *p_parent, HBoxContainer *p_toolbar);
    NoeAccountWorkspace();
    ~NoeAccountWorkspace();
};

#endif
